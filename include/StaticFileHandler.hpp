/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/10/22 19:59:31 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICFILEHANDLER_HPP
# define STATICFILEHANDLER_HPP

# include <string>
# include <sys/stat.h>
# include <dirent.h>
# include <map>
class HttpResponse;
class HttpRequest;
class Location;
class Context;

# define LOCATION_PATH	"./www/static"
# define INDEX_HTML		"index.html"


class	StaticFileHandler
{
	public:
		StaticFileHandler();
		~StaticFileHandler();
		StaticFileHandler(const StaticFileHandler& other);
		StaticFileHandler& operator=(const StaticFileHandler& other);

		HttpResponse	handleget(const Context& context);
		HttpResponse	handlepost(const Context& context);


		std::string		getFullPath() const;
		std::string		resolveMimeType(const std::string path) const;

	private:
		std::string							_handledPath;
		std::map<std::string, std::string>	_mimeTypes;


		void			_initMimeTypes();

		int				_verifyHeaders(const Context& context) const;
		int				_validateGetHeaders(const std::map<std::string, std::string>& headers) const;
		int				_validatePostHeaders(const Context& context, const std::map<std::string, std::string>& headers) const;
		int				_validateDeleteHeaders(const std::map<std::string, std::string>& headers) const;
		bool			_hasTargetHeader(const std::string& target, const std::map<std::string, std::string>& headers) const;


		HttpResponse	_handleDirListing(const Context& context);
		HttpResponse	_handleDirRequest(const Context& context);
		HttpResponse	_handleFileRequest(const Context& context);


		HttpResponse	_createResponseForFile(const Context& context) const;

		HttpResponse	_createDirListingResponse(const Context& context) const;
		std::string		_genDirListingHtml(const std::string& path) const;
		std::string		_genListing(const std::string& path) const;
		HttpResponse	_handleRoot(const Context& context);
		HttpResponse	_handleNotFound(const Context& context);

		std::string		_buildPathWithUri(const Context& context) const;
		std::string		_buildAbsolutePathWithRoot(const Context& context) const;
		std::string		_buildAbsolutePathWithIndex(const Context& context) const;

		void			_setHandledPath(const std::string& fullPath);
};

#endif
