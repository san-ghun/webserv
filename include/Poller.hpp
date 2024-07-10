/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/05 20:17:00 by sanghupa         ###   ########.fr       */
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
		struct t_event {
			Socket socket;
			short events;
		};
		
		Poller();
		~Poller();

		void						addSocket(const Socket socket, short events);
		void						removeSocket(const Socket socket);
		void						removeAllSockets();
		std::vector<struct pollfd>	getPollfds() const;
		std::vector<t_event>			poll(int timeout);

	private:
		std::vector<struct pollfd>	_pollfds;

		int							_findSocketIndex(int fd);
};

#endif