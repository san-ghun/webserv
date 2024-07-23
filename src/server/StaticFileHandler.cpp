/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:47 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticFileHandler.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Location.hpp"

////////////////////////////////////////////////////////////////////////////////
/// @brief Static member variable to store the MIME types.
/// Calls the `_staticInitMimeTypes` method to initialize the map
/// before the first object of this class is created.
std::map<std::string, std::string> StaticFileHandler::_staticMimeTypes;

////////////////////////////////////////////////////////////////////////////////

StaticFileHandler::StaticFileHandler()
{
	if (_staticMimeTypes.empty())
		_staticInitMimeTypes();
}

StaticFileHandler& StaticFileHandler::operator=(const StaticFileHandler& other)
{
	if (this != &other)
	{
		_fullPath = other._fullPath;
	}
	return (*this);
}

StaticFileHandler::~StaticFileHandler()
{}

////////////////////////////////////////////////////////////////////////////////
/// @brief handleRequest, public method to handle the request
/// This method is called by the RequestHandler to handle the request.
/// It checks if the requested URI is a directory or a file and calls the
/// appropriate method to handle the request.
/// @param request HttpRequest object as reference
/// @param location Location object as reference
HttpResponse StaticFileHandler::handleRequest(const HttpRequest& request, const Location& location)
{
	if (request.getUri() == "/")
		return (_handleRoot(location));
	_setFullPath(_buildFullPath(request.getUri()));
	if (isDir(_fullPath))
	{
		// if (/* && location.location.isListDir() */)
		// 	return (_handleDirListing(request, location));
		return (_handleDirRequest(request, location));
	}
	else if (isFile(_fullPath))
		return (_handleFileRequest(request, location));
	return (_handleNotFound());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief _handleDirRequest, private method to handle the directory request
/// This method is called when the directory listing is not `true`,
/// but the `uri` in the request is a directory.
/// the method modifies the request to append the default file name.
/// @warning this method depends on `_handleFileRequest` method
HttpResponse StaticFileHandler::_handleDirRequest(const HttpRequest& request, const Location& location)
{
	HttpRequest	modifiedRequest = request;
	modifiedRequest.setUri(_fullPath + INDEX_HTML);
	return (_handleFileRequest(modifiedRequest, location));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief _handleDirListing, private method to handle the directory listing
/// This method is called when the directory listing is `true`,
/// and the `uri` in the request is a directory.
/// @warning not implemented yet
HttpResponse StaticFileHandler::_handleDirListing(const HttpRequest& request, const Location& location)
{
	(void) request;
	(void) location;
	return (_createDirListingResponse());
}

// TODO: update this method to return a response object with the directory listing
/// @brief Create a `response` object for a directory listing.
/// @return `HttpResponse`
HttpResponse StaticFileHandler::_createDirListingResponse() const
{
	std::string			body = genDirListingHtml(getFullPath());
	std::string			bodyLength = getcontentLength(body);
	HttpResponse		resp;

	if (body.empty() || bodyLength.empty())
		return ;
	resp.setBody(body);
	resp.setDefaultHeaders();
	return (resp);
}

/*
	if (!file.is_open())
	{
		*this = notFound_404();
		return ("");
	}
	std::streamsize fileLength = file.tellg();
	if (fileLength <= 0)
	{
		*this = internalServerError_500();
		return ("");
	}
	file.seekg(0, std::ios::beg);
	body.resize(fileLength);
	if (!file.read(&body[0], fileLength))
	{
		*this = internalServerError_500();
		return ("");
	}
 */
/// @brief Generates an HTML page with a directory listing.
/// @param path 
/// @return Create a list of directories and files in HTML and export it to std::stringstream.
std::string	genDirListingHtml(const std::string& path)
{
	std::stringstream	html;
	std::string			body;

	html.clear();
    html << "<html><head><title>Directory Listing</title></head><body>";
    html << "<h2>Directory Listing for " << path << "</h2><ul>";
    
    // TODO: Implement directory listing

    html << "</ul></body></html>";
	body = html.str();
	return (body);
}

////////////////////////////////////////////////////////////////////////////////
HttpResponse StaticFileHandler::_handleFileRequest(const HttpRequest& request, const Location& location)
{
	(void)request;
	(void)location;
	return (_createResponseForFile());
}

HttpResponse StaticFileHandler::_createResponseForFile() const
{
	HttpResponse resp(_fullPath);
	resp.setHeader("Content-Type", resolveMimeType(_fullPath));
	return (resp);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief _handleRoot, private method to handle the root request
/// This method is called when the requested URI is the root directory.
/// It builds the full path of the default file and checks if the file exists.
HttpResponse StaticFileHandler::_handleRoot(const Location& location)
{
	_setFullPath(_buildPathWithRoot(location));
	if (!isFile(_fullPath))
		return (_handleNotFound());
	return (_createResponseForFile());
}

////////////////////////////////////////////////////////////////////////////////
std::string StaticFileHandler::_buildPathWithRoot(const Location& location) const
{
	std::string	root = location.getRootPath();
	std::string	defaultFile = 	location.getIndex();
	if (defaultFile.empty()) // if can not read from cong, give default value: hard code
		defaultFile = INDEX_HTML;
	std::string filePath = root + "/" + defaultFile;
	return (filePath);
}

std::string StaticFileHandler::_buildFullPath(const std::string& uri) const
{
	return(LOCATION_PATH + uri);
}

HttpResponse StaticFileHandler::_handleNotFound()
{
	return (HttpResponse::notFound_404());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Returns the MIME type of a given file based on its file extension.
/// @example resolveMimeType("index.html") ext = "html", and returns "text/html"
/// @param path The path of the file.
/// @return The MIME type of the file.
std::string StaticFileHandler::resolveMimeType(const std::string path) const
{
	std::string::size_type dotPos = path.find_last_of(".");
	if (dotPos == std::string::npos)
		return ("text/plain");
	std::string ext = path.substr(dotPos + 1);
	std::map<std::string, std::string>::const_iterator it = _staticMimeTypes.find(ext);
	if (it != _staticMimeTypes.end())
		return (it->second);
	return ("application/octet-stream");
}

////////////////////////////////////////////////////////////////////////////////
/// Setters

void	StaticFileHandler::_setFullPath(const std::string& fullPath)
{
	_fullPath = fullPath;
}

////////////////////////////////////////////////////////////////////////////////
/// Getters

std::string	StaticFileHandler::getFullPath() const
{
	return (_fullPath);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Initializes the MIME types map from the configuration.
/// If the configuration does not provide MIME types, default values are used.
void	StaticFileHandler::_staticInitMimeTypes()
{
	Config& config = Config::getInstance();

	if (config.getMimeTypeMap().size() > 0)
		_staticMimeTypes = config.getMimeTypeMap();
	else
	{
		_staticMimeTypes.insert(std::make_pair("html", "text/html"));
		_staticMimeTypes.insert(std::make_pair("css", "text/css"));
		_staticMimeTypes.insert(std::make_pair("js", "application/javascript"));
		_staticMimeTypes.insert(std::make_pair("png", "image/png"));
		_staticMimeTypes.insert(std::make_pair("jpg", "image/jpeg"));
		_staticMimeTypes.insert(std::make_pair("gif", "image/gif"));
		_staticMimeTypes.insert(std::make_pair("txt", "text/plain"));
	}
} 
