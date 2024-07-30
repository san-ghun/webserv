/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/24 00:35:43 by minakim          ###   ########.fr       */
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
		
		HttpResponse	handleRequest(const HttpRequest& request, const Location& location);

		std::string		getFullPath() const;
		std::string		resolveMimeType(const std::string path) const;

	private:
		static std::map<std::string, std::string>	_staticMimeTypes;
		std::string									_handledPath;

		static void		_staticInitMimeTypes();

		HttpResponse	_handleDirListing(const HttpRequest& request, const Location& location);
		HttpResponse	_handleDirRequest(const HttpRequest& request, const Location& location);
		HttpResponse	_handleFileRequest(const HttpRequest& request, const Location& location);


		HttpResponse	_createResponseForFile() const;
		HttpResponse	_createDirListingResponse() const;
	
		HttpResponse	_handleRoot(const Location& location);
		HttpResponse	_handleNotFound();

		std::string		_buildPathWithUri(const HttpRequest& request, const Location& location) const;
		std::string		_buildAbsolutePathWithRoot(const Location& location) const;
		std::string		_buildAbsolutePathWithIndex(const Location& location) const;


		void			_setHandledPath(const std::string& fullPath);
};

#endif
