/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 15:13:53 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include <sys/stat.h>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();

		HttpResponse	handleRequest(const HttpRequest request);

	private:
		static std::map<std::string, std::string>	_mimeTypes;

		std::string		getMimeType(const std::string path) const;
		bool			fileExists(const std::string path) const;
		static void		initializeMimeTypes();
};

#endif