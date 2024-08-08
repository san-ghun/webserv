/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/08 21:54:57 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include <sys/stat.h>
# include <map>

class HttpResponse;
class HttpRequest;
class Location;
class Context;

# define LOCATION_PATH	"./www/static"
# define INDEX_HTML		"index.html"

std::string	genDirListingHtml(const std::string& path);

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();
		StaticFileHandler(const StaticFileHandler& other);
		StaticFileHandler& operator=(const StaticFileHandler& other);

		HttpResponse	handleRequest(const Context& context);

		std::string		getFullPath() const;
		std::string		resolveMimeType(const std::string path) const;

	private:
		std::string							_handledPath;
		std::map<std::string, std::string>	_mimeTypes;


		void			_initMimeTypes(const Context& context);

		HttpResponse	_handleDirListing(const Context& context);
		HttpResponse	_handleDirRequest(const Context& context);
		HttpResponse	_handleFileRequest(const Context& context);


		HttpResponse	_createResponseForFile(const Context& context) const;
		HttpResponse	_createDirListingResponse(const Context& context) const;
	
		HttpResponse	_handleRoot(const Context& context);
		HttpResponse	_handleNotFound(const Context& context);

		std::string		_buildPathWithUri(const Context& context) const;
		std::string		_buildAbsolutePathWithRoot(const Context& context) const;
		std::string		_buildAbsolutePathWithIndex(const Context& context) const;


		void			_setHandledPath(const std::string& fullPath);
};

#endif
