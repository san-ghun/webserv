/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 15:53:22 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Poller.hpp"

void	Poller::addSocket(const Socket socket, short events)
{
	pollfd pfd;
	pfd.fd = socket.getFd();
	pfd.events = events;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
}

void	Poller::removeSocket(const Socket socket)
{
	_pollfds.erase(_pollfds.begin() + _findSocketIndex(socket.getFd()));
}

std::vector<struct pollfd>	Poller::getPollfds() const
{
	return (_pollfds);
}

std::vector<Poller::Event>	Poller::poll(int timeout)
{
	std::vector<struct pollfd>	pollfds = getPollfds();
	std::vector<Event>			events;
	if (::poll(pollfds.data(), pollfds.size(), timeout) < 0)
	{
		throw std::runtime_error("Poll failed");
	}
	for (size_t i = 0; i < pollfds.size(); i++)
	{
		if (pollfds[i].revents & POLLIN)
		{
			Event event;
			event.socket = Socket(pollfds[i].fd);
			event.events = pollfds[i].revents;
			events.push_back(event);
		}
	}
	return (events);
}

int	Poller::_findSocketIndex(int fd)
{
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == fd)
			return (i);
	}
	return (-1);
}
