/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:31:12 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

// ----------------------------------------------------

/// @brief Handle GET request by reading the file and returning an Http response.
/// @param path The path of the file to read.
/// @return The Http response containing the file contents.
/// In case the file is not found, a 404 Not Found response is returned.
static std::string handle_get(const std::string& path)
{
    // Prepend the current directory to the file path
    std::string file_path = "./www/static" + path;
    
    // Open the file
    std::ifstream ifs (file_path.data());
    
    // Check if the file is open
    if (!ifs.is_open())
	{
        // If not, return a 404 Not Found response
        return "Http/1.1 404 Not Found\r\n\r\n";
    }

    // Create a string stream to hold the Http response
    std::stringstream response;
    
    // Add the Http response headers
    response << "Http/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Connection: close\r\n\r\n";
    
    // Read the file contents into the response and return it
    response << ifs.rdbuf();
    return response.str();
}

/// @brief Parses an Http request and handles it.
/// @param request The Http request to parse and handle.
/// @return The Http response to send back to the client.
/// @throws None
static std::string handle_request(const std::string& request)
{
    // Parse the Http request
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
        // Handle other Http methods
        // Return a 405 Method Not Allowed response
        return "Http/1.1 405 Method Not Allowed\r\n\r\n";
    }
}

// ----------------------------------------------------

// TODO: Implement Config
Server::Server(Config& config)
	: _serverPort(config.getPort())
	, _maxBodySize(1024)
{
	_running = false;
	_serverNames = std::vector<std::string>();
	_serverNames.push_back(config.get("server_name"));
	_serverHost = config.getServerHost();
	_maxBodySize = config.getInt("max_body_size");
	_serverRoot = config.get("root");
	_serverIndex = config.get("default_file");
	_locations = std::vector<Location*>();

	std::map<std::string, Location*> locMap = config.getLocationMap();
	for (std::map<std::string, Location*>::iterator it = locMap.begin(); it != locMap.end(); it++)
		_locations.push_back(it->second);

	// _serverErrorPages = config.getErrorPageMap();
	_serverUploadPath = config.get("upload_dir");
	_serverCgiPath = config.get("cgi_dir");
}

Server::Server(int port)
	: _serverPort(port)
	, _maxBodySize(1024)
{}

Server::~Server()
{
	stop();
	// TODO: Delete all locations
}

/* 
void	Server::start()
{
	// TODO: Implement and replace stoi() function
	_listenSocket.bind(stoi(_serverPort));
	_listenSocket.listen();
	_listenSocket.set_nonblocking();
	_poller.addSocket(_listenSocket, POLLIN);

	_running = true;
	// Logger::info("Server started on port %d", port);

	while (_running) {
		std::vector<Poller::t_event> events = _poller.poll(1000);
		for (size_t i = 0; i < events.size(); i++ )
		{
			Poller::t_event event = events[i];
			if (event.socket.getFd() == _listenSocket.getFd()) {
				_handleNewConnection();
			} else {
				_handleClientData(event);
			}
		}
	}
}
*/

void	Server::start()
{
	try {
		// TODO: Implement and replace stoi() function
		_listenSocket.bind(_serverPort);
		// _listenSocket.bind(8080);
		_listenSocket.listen();
		_listenSocket.set_nonblocking();
		_pollfds.push_back((struct pollfd){_listenSocket.getFd(), POLLIN, 0});

		_running = true;
		// Logger::info("Server started on port %d", port);
		std::cout << "\rServer started on port " << _serverPort << "..." << std::endl;

		while (_running) 
		{
			if (g_sigint == true)
				break;
			int pollcount = poll(_pollfds.data(), _pollfds.size(), -1);
			if (pollcount < 0) {
				// std::cerr << "Error: poll failed" << std::endl;
				// Logger::Error("Server error: poll failed");
				break;
			}
			for (size_t i = 0; i < _pollfds.size(); i++) {
				if (_pollfds[i].revents & POLLIN) {
					if (_pollfds[i].fd == _listenSocket.getFd()) {
						_acceptNewConnection();
					} else {
						_handleClientData_2(_pollfds[i].fd, i);
					}
				}
			}
		}
		stop();
	} 
	catch (const std::exception& e)
	{
		// Print the error message
		std::cerr << "Error: " << e.what() << std::endl;
		// Logger::Error("Server error: %s", e.what());
		stop();
	}
}

void	Server::stop()
{
	if (_running)
	{
		_running = false;
		_listenSocket.close();
		_poller.removeAllSockets();
		_serverNames.clear();
		_locations.clear();
		_pollfds.clear();
		// Logger::info("Server stopped");
		std::cout << "\rServer stopped" << std::endl;
	}
}

void	Server::_acceptNewConnection()
{
	while (true)
	{
		int clientfd = accept(_listenSocket.getFd(), NULL, NULL);
		if (clientfd < 0)
		{
			// throw std::runtime_error("Failed to accept connection");
			break ;
		}
		
		// Set nonblocking mode
		int flags = fcntl(clientfd, F_GETFL, 0);
		if (flags == -1)
		{
			throw std::runtime_error("Failed to get file descriptor flags");
		}
		flags |= O_NONBLOCK;
		if (fcntl(clientfd, F_SETFL, flags) == -1)
		{
			throw std::runtime_error("Failed to set file descriptor flags to non-blocking");
		}

		_pollfds.push_back((struct pollfd){clientfd, POLLIN, 0});
	}
}

void	Server::_handleClientData_2(int clientSocket, size_t idx)
{
	char	buffer[1024];
	ssize_t	count = read(clientSocket, buffer, sizeof(buffer));
	if (count <= 0)
	{
		close(clientSocket);
		_pollfds.erase(_pollfds.begin() + idx);
		return ;
	}

	std::string	requestData(buffer, count);

	HttpRequest		request;


	if (!request.parse(requestData))
		return ; // throw?
	
	HttpResponse	response = _requestHandler.handleRequest(request);
	std::string		responseData = response.toString();
	// std::string	responseData = handle_request(requestData);


	std::cout << "TEST | start: handleClientData_2" << std::endl;
	std::cout << "TEST | " << requestData << std::endl;
	
	// Send the response
	write(clientSocket, responseData.c_str(), responseData.size());
	close(clientSocket);
	_pollfds.erase(_pollfds.begin() + idx);
}

void	Server::_handleNewConnection()
{
	// Suggestion: maybe use try-catch ?
	Socket	clientSocket = _listenSocket.accept();
	clientSocket.set_nonblocking();
	_poller.addSocket(clientSocket, POLLIN);
	// Logger::info("Client accepted: %d", clientSocket.getFd());
	std::cout << "\rClient accepted: " << clientSocket.getFd() << "..." << std::endl;
}

void	Server::_handleClientData(Poller::t_event event)
{
	// Suggestion: maybe use try-catch ?
	char	buffer[1024];
	ssize_t	count = read(event.socket.getFd(), buffer, sizeof(buffer));
	if (count <= 0)
	{
		_poller.removeSocket(event.socket);
		event.socket.close();
		return ;
	}

	std::string	requestData(buffer, count);
	/*
	HttpRequest	request;
	if (!request.parse(requestData))
	{
		_poller.removeSocket(event.socket);
		event.socket.close();
		return ;
	}
	
	// Process the request and send a response
	HttpResponse	response = _requestHandler.handleRequest(request);
	std::string	responseData = response.toString();
	*/
	std::string	responseData = handle_request(requestData);
	write(event.socket.getFd(), responseData.c_str(), responseData.size());
	_poller.removeSocket(event.socket);
	event.socket.close();
	/*
	if (request.isConnectionClose())
	{
		_poller.removeSocket(event.socket);
		event.socket.close();
	}
	*/
}

// Getters
std::vector<std::string>	Server::getServerNames() const
{
	return (_serverNames);
}

std::string	Server::getServerHost() const
{
	return (_serverHost);
}

// std::string	Server::getServerPort() const
// {
// 	return (std::to_string_serverPort));
// }

size_t	Server::getMaxBodySize() const
{
	return (_maxBodySize);
}

// size_t								Server::getMaxConnection() const {}

// size_t								Server::getMaxHeader() const {}

std::string	Server::getServerRoot() const
{
	return (_serverRoot);
}

std::string	Server::getServerIndex() const
{
	return (_serverIndex);
}

std::vector<Location*>	Server::getLocations() const
{
	return (_locations);
}

std::map<int, const std::string>	Server::getServerErrorPages() const
{
	return (_serverErrorPages);
}

std::string	Server::getServerUploadPath() const
{
	return (_serverUploadPath);
}

// std::string							Server::getServerAccessLogPath() const {}

// std::string							Server::getServerErrorLogPath() const {}

std::string	Server::getServerCgiPath() const
{
	return (_serverCgiPath);
}

// Setters
void	Server::setServerNames(std::vector<std::string> serverNames)
{
	_serverNames = serverNames;
}

void	Server::setServerHost(std::string serverHost)
{
	_serverHost = serverHost;
}

void	Server::setServerPort(int serverPort)
{
	_serverPort = serverPort;
}

void	Server::setMaxBodySize(size_t maxBodySize)
{
	_maxBodySize = maxBodySize;
}

// void								Server::setMaxConnection(size_t maxConnection) {}

// void								Server::setMaxHeader(size_t maxHeader) {}

void	Server::setServerRoot(std::string serverRoot)
{
	_serverRoot = serverRoot;
}

void	Server::setServerIndex(std::string serverIndex)
{
	_serverIndex = serverIndex;
}

void	Server::addLocation(Location* location)
{
	_locations.push_back(location);
}

void	Server::setServerErrorPages(std::map<int, const std::string> serverErrorPage)
{
	_serverErrorPages = serverErrorPage;
}

void	Server::setServerUploadPath(std::string serverUploadPath)
{
	_serverUploadPath = serverUploadPath;
}

// void								Server::setServerAccessLogPath(std::string serverAccessLogPath) {}

// void								Server::setServerErrorLogPath(std::string serverErrorLogPath) {}

void	Server::setServerCgiPath(std::string serverCgiPath)
{
	_serverCgiPath = serverCgiPath;
}
