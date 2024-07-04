/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/03 17:40:25 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP

# include <vector>
# include <sys/poll.h>
# include "Socket.hpp"

class	Poller
{
	public:
		struct Event {
			Socket socket;
			short events;
		};
		
		Poller();
		~Poller();

		void						addSocket(const Socket socket, short events);
		void						removeSocket(const Socket socket);
		std::vector<struct pollfd>	getPollfds() const;
		std::vector<Event>			poll(int timeout);

	private:
		std::vector<struct pollfd>	_pollfds;

		int							_findSocketIndex(int fd);
};

#endif