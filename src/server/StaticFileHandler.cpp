/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 14:52:10 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "StaticFileHandler.hpp"


StaticFileHandler::StaticFileHandler()
{
	if (_mimeTypes.empty())
		initializeMimeTypes();
}

StaticFileHandler::~StaticFileHandler()
{}

// TODO check the logic, how to work with path/request
HttpResponse StaticFileHandler::handleRequest(const HttpRequest request)
{
	std::string filePath = /* path + */ request.getUri();

	// test
	std::cout << "[test] file path: " << filePath << std::endl;

	if (!fileExists(filePath))
		return (HttpResponse::notFound_404());
	
	
	std::string mimeType = getMimeType(filePath);
	
	// test
	std::cout << "[test] mime type: " << mimeType << std::endl;

	HttpResponse resp = HttpResponse::fromFile(filePath);
	resp.setHeader(getMimeType(filePath), mimeType);

	return (resp);
}

// TODO test result, parse part
std::string StaticFileHandler::getMimeType(const std::string path) const
{
	std::string::size_type dotPosition = path.find_last_of(".");
	if (dotPosition == std::string::npos)
		return ("text/plain");
	std::string ext = path.substr(dotPosition);
	std::map<std::string, std::string>::const_iterator it = _mimeTypes.find(ext);
	if (it != _mimeTypes.end())
		return (it->second);
	return ("text/plain");
}

bool StaticFileHandler::fileExists(const std::string path) const
{
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
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