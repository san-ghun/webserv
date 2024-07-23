/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:23 by minakim          ###   ########.fr       */
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

void	genDirListingHtml(const std::string& path, std::stringstream& html);

class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();
		StaticFileHandler& operator=(const StaticFileHandler& other);

		HttpResponse	handleRequest(const HttpRequest& request, const Location& location);

		std::string		getFullPath() const;
		std::string		resolveMimeType(const std::string path) const;

	private:
		static std::map<std::string, std::string>	_staticMimeTypes;
		std::string									_fullPath;

		static void		_staticInitMimeTypes();

		HttpResponse	_handleDirListing(const HttpRequest& request, const Location& location);
		HttpResponse	_handleDirRequest(const HttpRequest& request, const Location& location);
		HttpResponse	_handleFileRequest(const HttpRequest& request, const Location& location);


		HttpResponse	_createResponseForFile() const;
		HttpResponse	_createDirListingResponse() const;
	
		HttpResponse	_handleRoot(const Location& location);
		HttpResponse	_handleNotFound();

		std::string		_buildFullPath(const std::string& uri) const;
		std::string		_buildPathWithRoot(const Location& location) const;

		void			_setFullPath(const std::string& fullPath);
};

#endif
