/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 21:05:14 by sanghupa          #+#    #+#             */
/*   Updated: 2024/06/23 00:12:12 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

// Sample code to mock webserv
// Code Start
/// ----------------------------------------
/// Sample code for utility
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int create_and_bind(int port) {
    int listen_sock;
    struct sockaddr_in server_addr;

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(listen_sock);
        return -1;
    }

    if (listen(listen_sock, SOMAXCONN) < 0) {
        perror("listen");
        close(listen_sock);
        return -1;
    }

    set_nonblocking(listen_sock);

    return listen_sock;
}


/// Sample code for request handling
std::string handle_get(const std::string& path) {
    std::string file_path = "." + path;
    std::ifstream ifs (file_path.data());
    if (!ifs.is_open()) {
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Connection: close\r\n\r\n";
    response << ifs.rdbuf();
    return response.str();
}

std::string handle_request(const std::string& request) {
    std::istringstream request_stream(request);
    std::string method, path, version;
    request_stream >> method >> path >> version;

	std::cout << "\r" << method << " | " << path << " | " << version << std::endl;

    if (method == "GET") {
        return handle_get(path);
    } else {
        return "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    }
}


/// Sample code for main server loop
void run_server(int port) {
    int listen_sock = create_and_bind(port);
    if (listen_sock == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

	std::cout << "\rserver listening on port " << port << "...\n" << std::endl;

    std::vector<pollfd> fds;
    fds.push_back((struct pollfd){listen_sock, POLLIN, 0});

    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1);
        if (poll_count == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == listen_sock) {
                    // Accept new connections
                    while (true) {
                        int client_sock = accept(listen_sock, NULL, NULL);
                        if (client_sock == -1) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break;
                            } else {
                                perror("accept");
                                break;
                            }
                        }

                        set_nonblocking(client_sock);
                        fds.push_back((struct pollfd){client_sock, POLLIN, 0});
                    }
                } else {
                    // Handle data from clients
                    char buffer[BUFSIZE];
                    ssize_t count = read(fds[i].fd, buffer, sizeof(buffer));
                    if (count == -1) {
                        if (errno != EAGAIN && errno != EWOULDBLOCK) {
                            perror("read");
                            close(fds[i].fd);
                            fds.erase(fds.begin() + i);
                            i--;  // Adjust index since we've modified the vector
                        }
                    } else if (count == 0) {
                        // Client closed connection
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;  // Adjust index
                    } else {
                        // Process the request and send a response
                        std::string request(buffer, count);
                        std::string response = handle_request(request);
                        write(fds[i].fd, response.c_str(), response.size());
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;  // Adjust index
                    }
                }
            }
        }
    }
}
/// ----------------------------------------
// Code End

static void	ft_sigint_handler(int sig)
{
	(void)sig;
	std::cout << "\rserver shutting down...\n" << std::endl;
	exit(0);
}

int	main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, ft_sigint_handler);

	run_server(8080);
	(void)argv;
	return (0);
}