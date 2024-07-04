/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 15:59:03 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();

		HttpResponse	handleRequest(const HttpRequest request);

	private:
		std::string		getMimeType(const std::string path) const;
		bool			fileExists(const std::string path) const;
};

#endif