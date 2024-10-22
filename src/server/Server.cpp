/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:46 by sanghupa          #+#    #+#             */
/*   Updated: 2024/10/22 22:54:10 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Context.hpp"

static unsigned int	ft_inet_addr(const std::string& ipAddr)
{
	std::string					octet;
	int							octets[4];
	std::string::size_type		pos[2] = {0};// index 0 for begining, 1 for end
	unsigned int				result = 0;

   for (int i = 0; i < 4; i++)
	{
		pos[1] = ipAddr.substr(pos[0]).find('.');
		if (pos[1] == std::string::npos) 
			pos[1] = ipAddr.size();
		octets[i] = atoi(ipAddr.substr(pos[0], pos[1]).c_str());
		pos[0] += pos[1] + 1;
	}
	for (size_t i = 0; i < 4; ++i)
		result |= (static_cast<unsigned int>(octets[i]) << ((3 - i) * 8));
	return (htonl(result));
}

Server::Server(Config& config)
	: _config(config)
{
	_running = false;
	// _listenSocket = Socket();
	// _poller = Poller();
	_serverConfigs = config.getServers();
}

Server::~Server()
{
	stop();
	// TODO: Delete all locations
}

void	Server::start()
{
	try {
		// Hosts and ports to manage
		for (size_t i = 0; i < _serverConfigs.size(); i++)
		{
			ListenInfo info = {
				_serverConfigs[i]->listen,
				_serverConfigs[i]->host,
				_serverConfigs[i]->port,
				-1
			};
			_listenInfos.push_back(info);
		}

		// Map to track clients
		// std::map<int, ListenInfo> clients;

		// Setup listen sockets
		for (size_t i = 0; i < _listenInfos.size(); i++)
		{
			int listen_socket = _setupListeningSocket(_listenInfos[i].host, _listenInfos[i].port);
			_listenInfos[i].fd = listen_socket;
			_pollfds.push_back((struct pollfd){listen_socket, POLLIN, 0});

			// Track listening sockets
			// clients[listen_socket] = _listenInfos[i];
			_clients[listen_socket] = _listenInfos[i];
			std::cout << "Listening on " << _listenInfos[i].host << ":" << _listenInfos[i].port << std::endl;
		}

		_running = true;

		while (_running) 
		{
			if (g_sigint == true)
				break;
			int pollcount = poll(_pollfds.data(), _pollfds.size(), -1);
			if (pollcount <= 0) 
			{
				// std::cerr << "Error: poll failed" << std::endl;
				// Logger::Error("Server error: poll failed");
				// break;
				continue ;
			}
			for (size_t i = 0; i < _pollfds.size(); i++)
			{
				int target = _pollfds[i].fd;
				if (_pollfds[i].revents & POLLIN)
				{
					// if (clients.find(target) != clients.end())
					if ((_clients.find(target) != _clients.end()) && (_clients[target].fd == target))
					{
						// New connection on a listening socket
						sockaddr_in client_addr;
						socklen_t client_addr_len = sizeof(client_addr);
						int client_socket = accept(target, (sockaddr*)&client_addr, &client_addr_len);
						// int client_socket = accept(target, NULL, NULL);
						if (client_socket < 0)
						{
							std::cerr << "Error: accept failed" << std::endl;
							// Logger::Error("Server error: accept failed");
							break ;
							// continue;
						}

						int flags = fcntl(client_socket, F_GETFL, 0);
						if (flags == -1)
						{
							throw std::runtime_error("Failed to get file descriptor flags");
						}
						flags |= O_NONBLOCK;
						if (fcntl(client_socket, F_SETFL, flags) < 0)
						{
							throw std::runtime_error("Failed to set file descriptor flags to non-blocking");
						}

						// Track new client connection
						_pollfds.push_back((struct pollfd){client_socket, POLLIN | POLLOUT, 0});
						// clients[client_socket] = clients[target];
						_clients[client_socket] = _clients[target];
						// std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;
						std::cout << "Client connected from " << _clients[client_socket].host << ":" << _clients[client_socket].port << std::endl;
						// Logger::info("Client connected from %s:%d", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					}
					else
					{
						// Data received on an existing client connection
						std::string requestData;
						char buffer[1024];
						ssize_t	count = read(target, buffer, sizeof(buffer));
						if (count <= 0)
						{
							close(target);
							// clients.erase(target);
							_clients.erase(target);
							_pollfds.erase(_pollfds.begin() + i);
							// --i;
							continue ;
						}

						// buffer[count] = '\0';

						////////////////////////////////////////////////////////////////////////////////////////
						std::cout << "TEST | start: handleClientData_2" << std::endl;
						// FIXME: need to handle bigger size of data, not only 1024
						std::string	requestData(buffer, count);

						// @author minakim
						// FIXME: @sanghupa please update this part

						ServerConfig&	serverConfig = _fetchConfig(target);
						HttpRequest		request(requestData);

						// test line: POST, bad request
						// request.setMethod("POST");
						// request.setContentLength(10);
						
						Context			contextFromTarget(serverConfig, request);
						HttpResponse	response = _requestHandler.handleRequest(contextFromTarget);
						std::string		responseData = response.generateResponseToString();
						// std::string	responseData = handle_request(requestData);

						std::cout << "TEST | " << requestData << std::endl;
						////////////////////////////////////////////////////////////////////////////////////////
						
						// Send the response
						write(target, responseData.c_str(), responseData.size());
						close(target);
						// clients.erase(target);
						_clients.erase(target);
						_pollfds.erase(_pollfds.begin() + i); 
						// --i;
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

/// @author minakim
/// FIXME:  @sanghupa please change or delete this method
ServerConfig& Server::_fetchConfig(int target)
{
	ServerConfig& serverConfig = *_config.getServerByListen(_clients[target].listen);	
	return (serverConfig);
}


void	Server::stop()
{
	if (_running)
	{
		_running = false;
		// _poller.removeAllSockets();
		_pollfds.clear();
		// Logger::info("Server stopped");
		std::cout << "\rServer stopped" << std::endl;
	}
}

bool	Server::isRunning() const
{
	return (_running);
}

int	Server::_setupListeningSocket(const std::string host, int port)
{
	// Create socket
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		throw std::runtime_error("Failed to create socket");
	}

	// Set socket options
	int optval = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		close(listenfd);
		throw std::runtime_error("Failed to set socket options");
	}

	// Bind socket
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_addr.s_addr = INADDR_ANY;
	std::cout << "host: " << host << ":" << port << std::endl;
	// inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
	// addr.sin_addr.s_addr = inet_addr(host.c_str());
	// std::cout << inet_addr(host.c_str()) << std::endl;
	addr.sin_addr.s_addr = ft_inet_addr(host);
	std::cout << ft_inet_addr(host) << std::endl;
	addr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		throw std::runtime_error("Failed to bind socket");
	}

	// Listen
	if (listen(listenfd, SOMAXCONN) < 0)
	{
		throw std::runtime_error("Failed to listen");
	}

	// Set nonblocking mode
	int flags = fcntl(listenfd, F_GETFL, 0);
	if (flags == -1)
	{
		throw std::runtime_error("Failed to get file descriptor flags");
	}
	flags |= O_NONBLOCK;
	if (fcntl(listenfd, F_SETFL, flags) < 0)
	{
		throw std::runtime_error("Failed to set file descriptor flags to non-blocking");
	}

	return (listenfd);
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

void	Server::_handleClientData(int clientSocket, size_t idx)
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
	HttpRequest		request(requestData);


	// if (!request.parse(requestData))
	// 	return ; // throw?
	
	// HttpResponse	response = _requestHandler.handleRequest(request, _fetchConfig(clientSocket));
	// std::string		responseData = response.toString();
	ServerConfig&	serverConfig = _fetchConfig(clientSocket);
	Context			contextFromTarget(serverConfig, request);
	HttpResponse	response = _requestHandler.handleRequest(contextFromTarget);
	std::string		responseData = response.generateResponseToString();
	std::cout << "TEST | start: handleClientData_2" << std::endl;
	std::cout << "TEST | " << requestData << std::endl;
	
	// Send the response
	write(clientSocket, responseData.c_str(), responseData.size());
	close(clientSocket);
	_pollfds.erase(_pollfds.begin() + idx);
}
