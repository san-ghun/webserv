/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/10 22:11:51 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"

// TODO: Implement Config
// Server::Server(Config config)
// 	: _serverPort(std::to_string(config.getPort()))
// 	, _maxBodySize(0)
// {}

Server::Server(int port)
	: _serverPort(std::to_string(port))
	, _maxBodySize(0)
{}

Server::~Server()
{
	stop();
	
	// TODO: Delete all locations
}

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

// Try-Catch implementation for start()
void	Server::start()
{
	try {
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
		// Logger::info("Server stopped");
	}
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

std::string	Server::getServerPort() const
{
	return (_serverPort);
}

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

void	Server::setServerPort(std::string serverPort)
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


void	Server::_handleNewConnection()
{
	// Suggestion: maybe use try-catch ?
	Socket	clientSocket = _listenSocket.accept();
	clientSocket.set_nonblocking();
	_poller.addSocket(clientSocket, POLLIN);
	// Logger::info("Client accepted: %d", clientSocket.getFd());
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
	HttpRequest	request;
	if (!request.parse(requestData))
	{
		_poller.removeSocket(event.socket);
		event.socket.close();
		return ;
	}
	
	// Process the request and send a response
	HttpResponse	response = _requestHandler.handleRequest(request);
	std::string		responseData = response.toString();
	write(event.socket.getFd(), responseData.c_str(), responseData.size());
	
	if (request.isConnectionClose())
	{
		_poller.removeSocket(event.socket);
		event.socket.close();
	}
}
