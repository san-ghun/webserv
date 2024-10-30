/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/07 17:31:17 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// # include "webserv.hpp"
# include <vector>
# include "Config.hpp"
# include "Location.hpp"
# include "RequestHandler.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Socket.hpp"
# include "Poller.hpp"

class	Config;
class	Location;
class	Socket;
class	Poller;
// class	RequestHandler;

struct	ServerConfig;

struct ListenInfo
{
	std::string listen;
	std::string	host;
	int			port;
	int			fd;
};

class	Server
{
	public:
		Server(Config& config);
		~Server();

		void						start();
		void						stop();

		bool						isRunning() const;

	private:
		bool						_running;
		Socket						_listenSocket;
		std::vector<ListenInfo>		_listenInfos;
		std::map<int, ListenInfo> 	_clients;
		Poller						_poller;
		std::vector<pollfd>			_pollfds;
		RequestHandler				_requestHandler;

		int							_setupListeningSocket(const std::string host, int port);

		void						_acceptNewConnection();
		void						_handleClientData(int clientSocket, size_t idx);
		ServerConfig&				_fetchConfig(int target);


		Config&						_config;
		std::vector<ServerConfig*>	_serverConfigs;
};

#endif
