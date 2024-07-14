/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/14 17:17:45 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "StaticFileHandler.hpp"

std::map<std::string, std::string> StaticFileHandler::_staticMimeTypes;

StaticFileHandler::StaticFileHandler()
{
	if (_staticMimeTypes.empty())
		_staticInitializeMimeTypes();
}

StaticFileHandler::~StaticFileHandler()
{}

HttpResponse StaticFileHandler::handleRequest(const HttpRequest request)
{
	if (request.getUri() == "/")
		return (_handleRoot());

	std::string filePath = _getFilePath(request.getUri());
	if (!_fileExists(filePath))
		return (_handleFileNotFound());
	std::string mimeType = _getMimeType(filePath);
	// HttpResponse resp(filePath);
	HttpResponse resp = HttpResponse::fromFile(filePath);
	resp.setHeader(_getMimeType(filePath), mimeType);

	return (resp);
}

HttpResponse StaticFileHandler::_handleRoot()
{
	return (HttpResponse::success_200());
}

std::string StaticFileHandler::_getFilePath(const std::string& uri) const
{
	return( LOCATION_FIXME + uri);
}

HttpResponse StaticFileHandler::_handleFileNotFound()
{
	return (HttpResponse::notFound_404());
}

/// @brief Returns the MIME type of a given file based on its file extension.
/// @param path The path of the file.
/// @return The MIME type of the file.

std::string StaticFileHandler::_getMimeType(const std::string path) const
{
	std::string::size_type dotPosition = path.find_last_of(".");
	if (dotPosition == std::string::npos)
		return ("text/plain");
	std::string ext = path.substr(dotPosition);
	std::map<std::string, std::string>::const_iterator it = _staticMimeTypes.find(ext);
	if (it != _staticMimeTypes.end())
		return (it->second);
	return ("application/octet-stream");
}

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool StaticFileHandler::_fileExists(const std::string path) const
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISREG(buffer.st_mode))
		return (true);
	return (false);
}

/// @brief Initializes the mimeTypes map with the default mime types.
void StaticFileHandler::_staticInitializeMimeTypes()
{
	_staticMimeTypes.insert(std::make_pair(".html", "text/html"));
	_staticMimeTypes.insert(std::make_pair(".css", "text/css"));
	_staticMimeTypes.insert(std::make_pair(".js", "application/javascript"));
	_staticMimeTypes.insert(std::make_pair(".png", "image/png"));
	_staticMimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
	_staticMimeTypes.insert(std::make_pair(".gif", "image/gif"));
	_staticMimeTypes.insert(std::make_pair(".txt", "text/plain"));
} 
