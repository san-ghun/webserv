/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 21:05:14 by sanghupa          #+#    #+#             */
/*   Updated: 2024/06/23 16:33:22 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int			set_nonblocking(int fd);
int			create_and_bind(int port);
std::string	handle_request(const std::string& request);
std::string	handle_get(const std::string& path);
void		run_server(int port);
void		acceptNewConnections(int listen_sock, std::vector<pollfd>& fds);
void		handleClientData(int client_sock, std::vector<pollfd>& fds, size_t idx);

/**
 * @brief Set a file descriptor to non-blocking mode.
 * 
 * @param fd The file descriptor.
 * @return The result of the operation. 0 on success, -1 on failure.
 * 
 * This function sets the file descriptor to non-blocking mode, allowing
 * for asynchronous I/O operations.
 */
int set_nonblocking(int fd)
{
    // Get the current file descriptor flags
    int flags = fcntl(fd, F_GETFL, 0);
    
    // Check if the operation was successful
    if (flags == -1) return -1;
    
    // Set the non-blocking flag
    flags |= O_NONBLOCK;
    
    // Set the file descriptor flags
    return fcntl(fd, F_SETFL, flags);
}

/**
 * @brief Create and bind a socket to a specific port.
 * 
 * @param port The port number to bind the socket to.
 * @return The socket file descriptor on success, -1 on failure.
 * 
 * This function creates a socket, sets the SO_REUSEADDR option, 
 * binds the socket to the specified port, and makes the socket non-blocking.
 */
int create_and_bind(int port)
{
    int listen_sock;
    struct sockaddr_in server_addr;

    // Create a socket
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0)
	{
        perror("socket");
        return -1;
    }

    // Set the SO_REUSEADDR option to allow reusing the address
    int opt = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        perror("setsockopt");
        close(listen_sock);
        return -1;
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
        perror("bind");
        close(listen_sock);
        return -1;
    }

    // Listen for incoming connections
    if (listen(listen_sock, SOMAXCONN) < 0)
	{
        perror("listen");
        close(listen_sock);
        return -1;
    }

    // Set the socket to non-blocking
    if (set_nonblocking(listen_sock) < 0)
	{
        perror("set_nonblocking");
        close(listen_sock);
        return -1;
    }

    return listen_sock;
}


/**
 * Parses an HTTP request and handles it.
 * 
 * @param request The HTTP request to parse and handle.
 * @return The HTTP response to send back to the client.
 * 
 * @throws None
 */
std::string handle_request(const std::string& request)
{
    // Parse the HTTP request
    std::istringstream request_stream(request);
    std::string method, path, version;
    request_stream >> method >> path >> version;

	// Log the request method, path, and version
    std::cout << "\r" << method << " | " << path << " | " << version << std::endl;

    if (method == "GET")
	{
        // Handle GET request
        return handle_get(path);
    } 
	else
	{
        // Handle other HTTP methods
        // Return a 405 Method Not Allowed response
        return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    }
}


/**
 * @brief Handle GET request by reading the file and returning an HTTP response.
 * 
 * @param path The path of the file to read.
 * @return The HTTP response containing the file contents.
 * 
 * In case the file is not found, a 404 Not Found response is returned.
 */
std::string handle_get(const std::string& path)
{
    // Prepend the current directory to the file path
    std::string file_path = "." + path;
    
    // Open the file
    std::ifstream ifs (file_path.data());
    
    // Check if the file is open
    if (!ifs.is_open())
	{
        // If not, return a 404 Not Found response
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    // Create a string stream to hold the HTTP response
    std::stringstream response;
    
    // Add the HTTP response headers
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Connection: close\r\n\r\n";
    
    // Read the file contents into the response and return it
    response << ifs.rdbuf();
    return response.str();
}


/**
 * Main server loop that listens for incoming connections and handles requests.
 * @param port The port number to listen on.
 */
void run_server(int port)
{
    int listen_sock = create_and_bind(port);
    if (listen_sock == -1)
	{
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Inform the user that the server is listening
    std::cout << "\rserver listening on port " << port << "...\n" << std::endl;

    std::vector<pollfd> fds;
    fds.push_back((struct pollfd){listen_sock, POLLIN, 0});

    // Main server loop
    while (true)
	{
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count == -1)
		{
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Process the events that occurred
        for (size_t i = 0; i < fds.size(); i++)
		{
            if (fds[i].revents & POLLIN)
			{
                if (fds[i].fd == listen_sock)
				{
                    // Accept new connections
                    acceptNewConnections(listen_sock, fds);
                }
				else
				{
                    // Handle data from clients
                    handleClientData(fds[i].fd, fds, i);
                }
            }
        }
    }
}

/**
 * Accepts new incoming connections and adds them to the list of file descriptors.
 * @param listen_sock The listening socket.
 * @param fds The vector of file descriptors.
 */
void acceptNewConnections(int listen_sock, std::vector<pollfd>& fds)
{
    while (true)
	{
        int client_sock = accept(listen_sock, NULL, NULL);
        if (client_sock == -1)
		{
            if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
                break;
            }
			else
			{
                perror("accept");
                break;
            }
        }

        set_nonblocking(client_sock);
        fds.push_back((struct pollfd){client_sock, POLLIN, 0});
    }
}

/**
 * Handles the data received from a client.
 * @param client_sock The socket of the client.
 * @param fds The vector of file descriptors.
 * @param idx The index of the client socket in the vector.
 */
void handleClientData(int client_sock, std::vector<pollfd>& fds, size_t idx)
{
    char buffer[BUFSIZE];
    ssize_t count = read(client_sock, buffer, sizeof(buffer));
    if (count == -1)
	{
        if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
            perror("read");
            close(client_sock);
            fds.erase(fds.begin() + idx);
        }
    }
	else if (count == 0)
	{
        // Client closed connection
        close(client_sock);
        fds.erase(fds.begin() + idx);
    }
	else
	{
        // Process the request and send a response
        std::string request(buffer, count);
        std::string response = handle_request(request);
        write(client_sock, response.c_str(), response.size());
        close(client_sock);
        fds.erase(fds.begin() + idx);
    }
}
/// ----------------------------------------
// Code End

/**
 * Handles the SIGINT signal by printing a message and exiting the program.
 * @param sig the signal number
 * @return void
 * @throws None
 */
static void	ft_sigint_handler(int sig)
{
	(void)sig;
	std::cout << "\rserver shutting down..." << std::endl;
	exit(0);
}

/**
 * The main function of the program.
 * It reads the port number from the command line arguments and runs the server.
 * 
 * @param argc The number of command line arguments.
 * @param argv An array of character pointers to command line arguments.
 * @return The exit status of the program.
 * 
 * The program expects one command line argument: the path to the configuration file.
 * If the argument is missing, the program prints the usage message and exits with status 1.
 * Otherwise, it runs the server on the specified port.
 */
int main(int argc, char *argv[])
{
	// Check if the correct number of arguments is provided
	if (argc != 2)
	{
		// Print the usage message and exit with status 1
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	// Ignore SIGPIPE signals
	signal(SIGPIPE, SIG_IGN);
	// Set the signal handler for SIGINT signal
	signal(SIGINT, ft_sigint_handler);

	// Run the server on the specified port
	run_server(8080);

	// Unused variable
	(void)argv;

	// try-catch block for error handling
	// Currently, it is commented out
	// try
	// {
	// 	// Create a Config object with the provided configuration file
	// 	Config config(argv[1]);
	// 	// Create a Server object with the Config object
	// 	Server server(config);
	// 	// Run the server
	// 	server.run();
	// }
	// catch (const std::exception &e)
	// {
	// 	// Print the error message and exit with status 1
	// 	std::cerr << "Error: " << e.what() << std::endl;
	// 	return (1);
	// }

	// Exit with status 0
	return (0);
}
