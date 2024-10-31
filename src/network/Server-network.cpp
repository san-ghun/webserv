/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server-network.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 22:15:38 by sanghupa          #+#    #+#             */
/*   Updated: 2024/10/30 23:00:08 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"

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
	// std::cout << "host: " << host << ":" << port << std::endl;
	// inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
	// addr.sin_addr.s_addr = inet_addr(host.c_str());
	// std::cout << inet_addr(host.c_str()) << std::endl;
	addr.sin_addr.s_addr = ft_inet_addr(host);
	// std::cout << ft_inet_addr(host) << std::endl;
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
