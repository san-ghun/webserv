/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 15:39:45 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"

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
	_listenSocket.bind(_port);
	_listenSocket.listen();
	_poller.addSocket(_listenSocket);

	_running = true;
	// Logger::info("Server started on port %d", port);

	while (running) {
		auto events = poller.poll();
		for (auto& event : events) {
			if (event.socket.getFd() == listenSocket.getFd()) {
				handleNewConnection();
			} else {
				handleClientData(event);
			}
		}
	}
}

void	Server::stop()
{}

// Getters
std::vector<std::string>	Server::getServerNames() const
{}

std::string					Server::getServerHost() const
{}

std::string					Server::getServerPort() const
{}

size_t						Server::getMaxBodySize() const
{}

// size_t								Server::getMaxConnection() const {}

// size_t								Server::getMaxHeader() const {}

std::string							Server::getServerRoot() const
{}

std::string							Server::getServerIndex() const
{}

std::vector<Location*>				Server::getLocations() const
{}

std::map<int, const std::string>	Server::getServerErrorPages() const
{}

std::string							Server::getServerUploadPath() const
{}

// std::string							Server::getServerAccessLogPath() const {}

// std::string							Server::getServerErrorLogPath() const {}

std::string							Server::getServerCgiPath() const
{}

// Setters
void								Server::setServerNames(std::vector<std::string> serverNames)
{}

void								Server::setServerHost(std::string serverHost)
{}

void								Server::setServerPort(std::string serverPort)
{}

void								Server::setMaxBodySize(size_t maxBodySize)
{}

// void								Server::setMaxConnection(size_t maxConnection) {}

// void								Server::setMaxHeader(size_t maxHeader) {}

void								Server::setServerRoot(std::string serverRoot)
{}

void								Server::setServerIndex(std::string serverIndex)
{}

void								Server::addLocation(Location* location)
{}

void								Server::setServerErrorPages(std::map<int, const std::string> serverErrorPage)
{}

void								Server::setServerUploadPath(std::string serverUploadPath)
{}

// void								Server::setServerAccessLogPath(std::string serverAccessLogPath) {}

// void								Server::setServerErrorLogPath(std::string serverErrorLogPath) {}

void								Server::setServerCgiPath(std::string serverCgiPath)
{}


void								Server::_handleNewConnection()
{}

void								Server::_handleClientData(Poller::Event event)
{}

/**
bool								_running
Socket								_listenSocket
Poller								_poller
RequestHandler						_requestHandler

std::vector<std::string>			_serverNames
std::string							_serverHost
std::string							_serverPort
size_t								_maxBodySize
// size_t								_maxConnection
// size_t								_maxHeader
std::string							_serverRoot
std::string							_serverIndex
std::vector<Location*>				_locations
std::map<int, const std::string>	_serverErrorPages
std::string							_serverUploadPath
// std::string							_serverAccessLogPath
// std::string							_serverErrorLogPath
std::string							_serverCgiPath
*/
