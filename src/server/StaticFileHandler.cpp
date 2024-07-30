/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticFileHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/30 14:09:23 by minakim          ###   ########.fr       */
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

StaticFileHandler::StaticFileHandler(const StaticFileHandler& other)
{
	_handledPath = other._handledPath;
}

StaticFileHandler& StaticFileHandler::operator=(const StaticFileHandler& other)
{
	if (this != &other)
	{
		_handledPath = other._handledPath;
	}
	return (*this);
}

StaticFileHandler::~StaticFileHandler()
{}

////////////////////////////////////////////////////////////////////////////////
/// Public methods: handleRequest
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
	_setHandledPath(_buildPathWithUri(request, location));
	if (isDir(_handledPath))
	{
		// if (/* && location.location.isListDir() */)
		// 	return (_handleDirListing(request, location));
		return (_handleDirRequest(request, location));
	}
	if (isFile(_handledPath))
		return (_handleFileRequest(request, location));
	return (_handleNotFound());
}

////////////////////////////////////////////////////////////////////////////////
/// Private methods
////////////////////////////////////////////////////////////////////////////////

/// @brief _handleDirRequest, private method to handle the directory request
/// This method is called when the directory listing is not `true`,
/// but the `uri` in the request is a directory.
/// the method modifies the request to append the default file name.
/// @warning this method depends on `_handleFileRequest` method
HttpResponse StaticFileHandler::_handleDirRequest(const HttpRequest& request, const Location& location)
{
	HttpRequest	modifiedRequest = request;

	std::string absolutePath = _buildAbsolutePathWithIndex(location);
	_setHandledPath(absolutePath);

	std::cout << "TEST |  absolute path that with index is built: " << _handledPath << std::endl;
	
	if (!isFile(_handledPath))
		return (_handleNotFound());
	modifiedRequest.setUri(getFullPath());
	return (_handleFileRequest(modifiedRequest, location));
}

std::string StaticFileHandler::_buildAbsolutePathWithIndex(const Location& location) const
{
	std::string index = INDEX_HTML;
	if (!location.getIndex().empty())
		index = location.getIndex();
	return (_handledPath + index);
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
	HttpResponse		resp;
	resp.setBody(genDirListingHtml(getFullPath()));

	if (resp.getBody().empty() || resp.getBodyLength() <= 0)
		return (HttpResponse::internalServerError_500());
	resp.setDefaultHeaders();
	return (resp);
}

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
	HttpResponse resp(_handledPath);
	resp.setHeader("Content-Type", resolveMimeType(_handledPath));
	return (resp);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief _handleRoot, private method to handle the root request
/// This method is called when the requested URI is the root directory.
/// It builds the full path of the default file and checks if the file exists.
HttpResponse StaticFileHandler::_handleRoot(const Location& location)
{
	_setHandledPath(_buildAbsolutePathWithRoot(location));
	if (!isFile(_handledPath))
		return (_handleNotFound());
	return (_createResponseForFile());
}

////////////////////////////////////////////////////////////////////////////////
std::string StaticFileHandler::_buildAbsolutePathWithRoot(const Location& location) const
{
	std::string	root		= location.getRootPath();
	std::string	defaultFile = location.getIndex();
	std::string fullPath;

	if (root.empty())
		throw std::runtime_error("Root path is empty");
	if (defaultFile.empty()) // if can not read from cong, give default value: hard code
		defaultFile = INDEX_HTML;
	fullPath = "." + root + "/" + defaultFile;
	std::cout << "TEST | absolute path that with index is built: " << _handledPath << std::endl;
	return (fullPath);
}

std::string StaticFileHandler::_buildPathWithUri(const HttpRequest& request, const Location& location) const
{
	std::string	root = location.getRootPath();
	std::string fullPath;
	if (root.empty())
		throw std::runtime_error("Root path is empty");
	fullPath = "." + root + request.getUri();

	std::cout << "TEST | first path that is built: " << _handledPath << std::endl;

	return(fullPath);
}

HttpResponse StaticFileHandler::_handleNotFound()
{
	return (HttpResponse::notFound_404());
}


////////////////////////////////////////////////////////////////////////////////
/// Setters
////////////////////////////////////////////////////////////////////////////////

void	StaticFileHandler::_setHandledPath(const std::string& fullPath)
{
	_handledPath = fullPath;
}

////////////////////////////////////////////////////////////////////////////////
/// Getters
////////////////////////////////////////////////////////////////////////////////

std::string	StaticFileHandler::getFullPath() const
{
	return (_handledPath);
}

////////////////////////////////////////////////////////////////////////////////
/// Mime types
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