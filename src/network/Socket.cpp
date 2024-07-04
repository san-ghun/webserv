/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:23:55 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/03 20:25:05 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Socket.hpp"

Socket::Socket()
	: _socketfd(-1)
{
	_createSocket();
}

Socket::Socket(int fd)
	: _socketfd(fd)
{
	if (fd < 0)
	{
		throw std::invalid_argument("Invalid socket file descriptor");
	}
}

Socket::~Socket()
{
	close();
}

void	Socket::_createSocket()
{
	_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketfd < 0)
	{
		throw std::runtime_error("Failed to create socket");
	}
	_setSocketOptions();
}

void	Socket::_setSocketOptions()
{
	int opt = 1;
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		throw std::runtime_error("Failed to set socket options");
	}
}

void	Socket::_bindAddress(int port)
{
	memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(port);
	if (::bind(_socketfd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
	{
		throw std::runtime_error("Failed to bind socket");
	}
}

void	Socket::bind(int port)
{
	_bindAddress(port);
}

void	Socket::listen()
{
	if (::listen(_socketfd, SOMAXCONN) < 0)
	{
		throw std::runtime_error("Failed to listen on socket");
	}
}

void	Socket::set_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		throw std::runtime_error("Failed to get file descriptor flags");
	}
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
	{
		throw std::runtime_error("Failed to set file descriptor flags to non-blocking");
	}
}

Socket	Socket::accept()
{
	int clientfd = ::accept(_socketfd, NULL, NULL);
	if (clientfd < 0)
	{
		throw std::runtime_error("Failed to accept connection");
	}
	return Socket(clientfd);
}

void	Socket::connet(const std::string host, int port)
{}

ssize_t	Socket::recv(char* buf, size_t len)
{}

ssize_t	Socket::send(const char* buf, size_t len)
{}

void	Socket::close()
{
	if (_socketfd >= 0)
	{
		::close(_socketfd);
		_socketfd = -1;
	}
}

int		Socket::getFd() const
{
	return _socketfd;
}
