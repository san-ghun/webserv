/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:23:55 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 15:57:02 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <cstring>
# include <netinet/in.h>

class	Socket
{
	public:
		Socket();
		explicit Socket(int fd);
		~Socket();

		void		bind(int port);
		void		listen();
		void		set_nonblocking();
		Socket		accept();
		void		connet(const std::string host, int port);
		ssize_t		recv(char* buf, size_t len);
		ssize_t		send(const char* buf, size_t len);
		void		close();

		int			getFd() const;

	private:
		int			_socketfd;
		sockaddr_in	_address;

		void		_createSocket();
		void		_setSocketOptions();
		void		_bindAddress(int port);
};

#endif