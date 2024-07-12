/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/10 21:23:36 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// # include "webserv.hpp"
# include <vector>
// # include "Config.hpp"
# include "Location.hpp"
# include "Socket.hpp"
# include "Poller.hpp"
# include "RequestHandler.hpp"

// class	Config;
class	Location;
class	Socket;
class	Poller;
// class	RequestHandler;

class	Server
{
	public:
		// Server(Config config);
		Server(int port);
		~Server();

		void								start();
		void								stop();

		// Getters
		std::vector<std::string>			getServerNames() const;
		std::string							getServerHost() const;
		std::string							getServerPort() const;
		size_t								getMaxBodySize() const;
		// size_t								getMaxConnection() const;
		// size_t								getMaxHeader() const;
		std::string							getServerRoot() const;
		std::string							getServerIndex() const;
		std::vector<Location*>				getLocations() const;
		std::map<int, const std::string>	getServerErrorPages() const;
		std::string							getServerUploadPath() const;
		// std::string							getServerAccessLogPath() const;
		// std::string							getServerErrorLogPath() const;
		std::string							getServerCgiPath() const;
		// Setters
		void								setServerNames(std::vector<std::string> serverNames);
		void								setServerHost(std::string serverHost);
		void								setServerPort(std::string serverPort);
		void								setMaxBodySize(size_t maxBodySize);
		// void								setMaxConnection(size_t maxConnection);
		// void								setMaxHeader(size_t maxHeader);
		void								setServerRoot(std::string serverRoot);
		void								setServerIndex(std::string serverIndex);
		void								addLocation(Location* location);
		void								setServerErrorPages(std::map<int, const std::string> serverErrorPage);
		void								setServerUploadPath(std::string serverUploadPath);
		// void								setServerAccessLogPath(std::string serverAccessLogPath);
		// void								setServerErrorLogPath(std::string serverErrorLogPath);
		void								setServerCgiPath(std::string serverCgiPath);

	private:
		bool								_running;
		Socket								_listenSocket;
		Poller								_poller;
		std::vector<pollfd>					_pollfds;
		// RequestHandler						_requestHandler;

		void								_handleNewConnection();
		void								_handleClientData(Poller::t_event event);

		void								_acceptNewConnection();
		void								_handleClientData_2(int clientSocket, size_t idx);

		std::vector<std::string>			_serverNames;
		std::string							_serverHost;
		std::string							_serverPort;
		size_t								_maxBodySize;
		// size_t								_maxConnection;
		// size_t								_maxHeader;
		std::string							_serverRoot;
		std::string							_serverIndex;
		std::vector<Location*>				_locations;
		std::map<int, const std::string>	_serverErrorPages;
		std::string							_serverUploadPath;
		// std::string							_serverAccessLogPath;
		// std::string							_serverErrorLogPath;
		std::string							_serverCgiPath;
};

#endif