/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/11/07 15:16:30 by minakim          ###   ########.fr       */
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

		HttpResponse	handleGet(const Context& context);
		HttpResponse	handlePost(const Context& context);
		HttpResponse	handleDelete(const Context& context);		


		std::string		getRelativePath() const;
		std::string		resolveMimeType(const std::string path) const;

	private:
		/// @brief 							relativePath to clarify that
		///									it’s constructed relative to the server root
		///									and location.
		std::string							_relativePath;
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

		std::string		_buildRelativePathWithUri(const Context& context) const;
		std::string		_buildRelativePathWithRootAndIndex(const Context& context) const;
		std::string		_buildRelativePathWithIndex(const Context& context) const;

		void			_setRelativePath(const std::string& resolvedPath);
};

#endif
