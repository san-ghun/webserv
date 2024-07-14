/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/14 16:44:21 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include <sys/stat.h>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

// FIXME: Delete when correct logic is applied in the future
# define LOCATION_FIXME	"./www/static"
# define LOOT_DIR		"./www/static"

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();

		HttpResponse	handleRequest(const HttpRequest request);

	private:
		static std::map<std::string, std::string>	_staticMimeTypes;

		std::string		_getMimeType(const std::string path) const;
		bool			_fileExists(const std::string path) const;
		static void		_staticInitializeMimeTypes();
		HttpResponse	_handleRoot();
		std::string		_getFilePath(const std::string& uri) const;
		HttpResponse	_handleFileNotFound();
};

#endif
