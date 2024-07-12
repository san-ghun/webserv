/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 20:18:24 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "StaticFileHandler.hpp"

std::map<std::string, std::string> StaticFileHandler::_mimeTypes;

StaticFileHandler::StaticFileHandler()
{
	if (_mimeTypes.empty())
		initializeMimeTypes();
}

StaticFileHandler::~StaticFileHandler()
{}

HttpResponse StaticFileHandler::handleRequest(const HttpRequest request)
{
	std::string filePath = FOLDER_PATH + request.getUri();
	// std::cout << "filePath: " << filePath << std::endl;
	
	if (!fileExists(filePath))
		return (HttpResponse::notFound_404());
	std::string mimeType = getMimeType(filePath);

	HttpResponse resp = HttpResponse::fromFile(filePath);
	resp.setHeader(getMimeType(filePath), mimeType);

	return (resp);
}

std::string StaticFileHandler::getMimeType(const std::string path) const
{
	std::string::size_type dotPosition = path.find_last_of(".");
	if (dotPosition == std::string::npos)
		return ("text/plain");
	std::string ext = path.substr(dotPosition);
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(ext);
	if (it != _mimeTypes.end())
		return (it->second);
	return ("application/octet-stream");
}

bool StaticFileHandler::fileExists(const std::string path) const
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
        return (false);
	if (S_ISREG(buffer.st_mode))
        return (true);
	return (false);
}

/// @brief Initializes the mimeTypes map with the default mime types.
void StaticFileHandler::initializeMimeTypes()
{
	_mimeTypes.insert(std::make_pair(".html", "text/html"));
	_mimeTypes.insert(std::make_pair(".css", "text/css"));
	_mimeTypes.insert(std::make_pair(".js", "application/javascript"));
	_mimeTypes.insert(std::make_pair(".png", "image/png"));
	_mimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
	_mimeTypes.insert(std::make_pair(".gif", "image/gif"));
	_mimeTypes.insert(std::make_pair(".txt", "text/plain"));
} 