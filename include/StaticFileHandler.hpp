/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/03 17:42:45 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();

		HTTPResponse	handleRequest(const HTTPRequest request);

	private:
		std::string		getMimeType(const std::string path) const;
		bool			fileExists(const std::string path) const;
};

#endif