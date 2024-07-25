/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/25 15:40:55 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpResponse.hpp"

////////////////////////////////////////////////////////////////////////////////
/// Your HTTP response status codes must be accurate.
/// You server must have default error pages if none are provided.
/// Limit client body size.
////////////////////////////////////////////////////////////////////////////////


/// @brief Default constructor for the HttpResponse class.
/// Initializes the status code to 200 and the status message to "OK".
HttpResponse::HttpResponse()
	: _statusCode(200), _statusMessage("OK")
{
}

/// @brief Constructor for the HttpResponse class with file path.
/// Initializes the status code to 200 and the status message to "OK".
/// @param filePath 
HttpResponse::HttpResponse(const std::string& filePath)
	:_statusCode(200), _statusMessage("OK")
{
	initializefromFile(filePath);
}

/// @brief Copy constructor for the HttpResponse class.
/// @param other 
HttpResponse::HttpResponse(const HttpResponse& other)
{
	_statusCode = other._statusCode;
	_statusMessage = other._statusMessage;
	_headers = other._headers;
	_body = other._body;

}

/// @brief Assignment operator for the HttpResponse class.
/// @param other 
HttpResponse& HttpResponse::operator=(const HttpResponse& other)
{
	if (this != &other)
	{
		_statusCode = other._statusCode;
		_statusMessage = other._statusMessage;
		_headers = other._headers;
		_body = other._body;
	}
	return (*this);
}
/// @brief Destructor for the HttpResponse class.
HttpResponse::~HttpResponse()
{
}

////////////////////////////////////////////////////////////////////////////////
std::string	HttpResponse::toString() const
{
	return (getResponseLine() + _getHeadersString() + "\r\n\r\n" + _body);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Use in static function to set the default headers for an HttpResponse object.
/// @param resp The object to set the headers for.
/// @param bodyContent The content to be included in the response body.
void	HttpResponse::setDefaultHeaders(HttpResponse& resp)
{
	resp._setDefaultHeadersImpl();
}

/// @brief Read the Object's body and set the default headers for the response.
/// @param bodyContent The content to be included in the response body.
void	HttpResponse::setDefaultHeaders()
{
	this->_setDefaultHeadersImpl();
}

/// @brief Basic implementation of setting the default headers for an HttpResponse object.
void	HttpResponse::_setDefaultHeadersImpl()
{
	std::string	bodyLength = getcontentLength(this->getBody());
	if (bodyLength.empty())
		*this = HttpResponse::internalServerError_500();
	this->setHeader("Content-Length", bodyLength);
	this->setHeader("Content-Type", "text/html");
	this->setHeader("Connection", "close");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Creates an Static HttpResponse object by reading the contents of a file.
/// @param filePath The path to the file to be read.
/// @return HttpResponse The created HttpResponse object.
/// @warning If the file cannot be opened, the response point a 404 error().
void	HttpResponse::initializefromFile(const std::string& filePath)
{
	std::string body = _fileToString(filePath);
	std::string bodyLength = getcontentLength(body);

	if (body.empty() || bodyLength.empty())
		return ;
	setBody(body);
	setDefaultHeaders();
}

/// @brief Reads the contents of a file into a string.
/// If the file cannot be opened, the response point a 404 error().
/// If the file is empty, the response point a 500 error().
/// @param filePath The path to the file to be read.
/// @return Return the file content as a string.
/// If there is any error, return an empty string.
std::string HttpResponse::_fileToString(const std::string& filePath)
{
	std::ifstream	file(filePath.c_str(), std::ios::binary | std::ios::ate);
	std::string		body;
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
	return (body);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Returns the headers as a string.
/// @return std::string, The headers as a string.
std::string	HttpResponse::_getHeadersString() const
{
	std::string headers;
	for (std::map<std::string, std::string>::const_iterator it =_headers.begin();
															it != _headers.end();
															it++)
	{
		headers += it->first + ": " + it->second + "\r\n";
	}
	return (headers);
}

/// @brief returns the status line as a string.
/// @return std::string, The status line as a string.
std::string HttpResponse::_getStatusLine() const
{
	std::stringstream statusLine;
	statusLine << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	return (statusLine.str());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Initializes the map of HTTP status codes to status messages.
/// @var statusMap Static, The map of status codes to status messages.
/// @return Return the map of status codes and status messages.
const std::map<int, std::string>& HttpResponse::_StaticInitStatusMap()
{
	static std::map<int, std::string> statusMap;

	if (statusMap.empty())
	{
		statusMap[200] = "OK";
		statusMap[201] = "Created";
		statusMap[204] = "No Content";
		statusMap[301] = "Moved Permanently";
		statusMap[302] = "Found";
		statusMap[400] = "Bad Request";
		statusMap[401] = "Unauthorized";
		statusMap[403] = "Forbidden";
		statusMap[404] = "Not Found";
		statusMap[405] = "Method Not Allowed";
		statusMap[408] = "Request Timeout";
		statusMap[413] = "Request Entity Too Large";
		statusMap[418] = "I'm a Teapot";
		statusMap[500] = "Internal Server Error";
		statusMap[501] = "Not Implemented";
	}
	return (statusMap);
}

/// @brief Sets the HTTP status code and corresponding message for the response.
/// 
/// This function sets the `_statusCode` member variable and updates the `_statusMessage`
/// based on a predefined map of status codes. The map is initialized once and reused, 
/// ensuring a consistent set of status codes and messages across the application.
///
/// If the provided status code is not found in the map, an exception is thrown.
///
/// @param code The HTTP status code to set for the response.
/// @throws std::runtime_error if the status code is not found in the map.
void	HttpResponse::setStatusCode(int code)
{
	_statusCode = code;
	const std::map<int, std::string>& statusMap = _StaticInitStatusMap();

	std::map<int, std::string>::const_iterator it = statusMap.find(code);
	if (it != statusMap.end())
		_statusMessage = it->second;
	else
		throw std::runtime_error("Unknown status code");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Retrieves error page data for a given HTTP status code.
///
/// This function returns the error page data, including the full path and validity of the page file.
/// It first checks if the status code is within the valid range (400-599). If not, it throws an exception.
/// If the data for the given code is not already cached, it fetches the page path from the configuration,
/// constructs the full path, checks if the file exists, and caches the result.
///
/// @param code The HTTP status code for which to fetch the error page data.
/// @return The `t_page_detail` containing the full path and validity of the error page file.
/// @throws std::runtime_error if the status code is not in the valid range.
t_page_detail	fetchPageData(int code)
{
	static std::map<int, t_page_detail> pageMap;
	
	if (code < 400 || code > 599)
		throw std::runtime_error("Invalid error code");

	std::map<int, t_page_detail>::iterator it = pageMap.find(code);
	if (it == pageMap.end())
	{
		Config&		config = Config::getInstance();
		std::string	fullPath = getFullErrorPath(config.getErrorPage(code));
		pageMap[code] = buildOnePage(fullPath);
	}
	return (pageMap[code]);
}

t_page_detail	buildOnePage(const std::string& path)
{
	t_page_detail	page;
	page.path = path;
	page.isValid = isFile(path);
	return (page);
}


std::string	getFullErrorPath(const std::string& path)
{
	if (path.empty())
		return ("");
	return (LOCATION_ROOT_PATH + path);
}

////////////////////////////////////////////////////////////////////////////////
/// Static functions: Helper functions to create common HTTP responses.

/// @brief Creates an HTTP response object for a specified error code.
///
/// This function constructs an `HttpResponse` object based on the given error code. It first attempts
/// to fetch error page data using the `fetchPageData` function. If a valid error page file is found, it
/// uses that file to create the response. If the error page is missing or invalid, it generates a simple
/// HTML response indicating an error.
///
/// @param code The HTTP error code for which to create the response. It should be in the range 400-599.
/// @return An `HttpResponse` object configured with either the error page content or a default error message.
/// @throws std::runtime_error if the provided error code is out of the valid range (400-599).
HttpResponse HttpResponse::_createErrorResponse(int code)
{
	if (code < 400 || code > 599)
		throw std::runtime_error("Invalid error code");

	t_page_detail pageData = fetchPageData(code);
	if (pageData.path.empty() || !pageData.isValid)
		return (_createSimpleHttpResponse(code));

	HttpResponse resp(pageData.path);
	resp.setStatusCode(code);
	return (resp);
}

/// @brief Generates a simple HTML response for a given HTTP status code.
/// @param code The HTTP status code for which to generate the response. 
/// @return Return the generated HTML response.
HttpResponse	HttpResponse::_createSimpleHttpResponse(int code)
{
	HttpResponse resp;
	resp.setStatusCode(code);
	resp.setBody(resp._generateHtmlBody());
	setDefaultHeaders(resp);
	return (resp);
}

/// @brief Generates a simple HTML response body for a given HTTP status code.
/// @return Return the generated HTML body as a string.
std::string	HttpResponse::_generateHtmlBody()
{
	if (getStatusCode().empty() || getStatusMessage().empty())
		return ("");
	std::string body = 
		"<html>"
		"<body>"
		"<h1>" + getStatusCode() + " " + getStatusMessage() + "</h1>"
		"</body>"
		"</html>";
	return (body);
}

////////////////////////////////////////////////////////////////////////////////
/// Static functions: Common HTTP responses.
HttpResponse	HttpResponse::badRequest_400()
{
	return (_createErrorResponse(400));
}

HttpResponse	HttpResponse::forbidden_403()
{
	return (_createErrorResponse(403));
}

HttpResponse	HttpResponse::notFound_404()
{
	return ( _createErrorResponse(404));
}

HttpResponse	HttpResponse::methodNotAllowed_405()
{
	return (_createErrorResponse(405));
}

HttpResponse	HttpResponse::requestTimeout_408()
{
	return (_createErrorResponse(408));
}

HttpResponse	HttpResponse::requestEntityTooLarge_413()
{
	return (_createErrorResponse(413));
}

HttpResponse	HttpResponse::imaTeapot_418()
{
	return (_createErrorResponse(418));
}

HttpResponse	HttpResponse::internalServerError_500()
{
	return (_createErrorResponse(500));
}

HttpResponse	HttpResponse::notImplemented_501()
{
	return (_createErrorResponse(501));
}

/// @brief Creates a successful HTTP response with status code 200.
/// @details All `Response` object constructors initialize the status code to 200 and the status message to "OK".
/// @return Return the generated HTML response.
HttpResponse	HttpResponse::success_200()
{
	HttpResponse resp;

	resp.setBody(resp._generateHtmlBody());
	setDefaultHeaders(resp);
	return (resp);
}

////////////////////////////////////////////////////////////////////////////////
/// Setters

void	HttpResponse::setStatusCode(int code, const std::string statusMessage)
{
	_statusCode = code;
	_statusMessage = statusMessage;
}

void	HttpResponse::setHeader(const std::string key, const std::string value)
{
	_headers.insert(std::pair<std::string, std::string>(key, value));
}

void	HttpResponse::setBody(const std::string bodyContent)
{
	_body = bodyContent;
}

////////////////////////////////////////////////////////////////////////////////
/// Getters

std::string	HttpResponse::getBody()
{
	return (_body);
}

std::string HttpResponse::getResponseLine() const
{
	std::ostringstream oss;
	oss << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	return (oss.str());
}

std::string HttpResponse::getStatusCode() const
{
	std::ostringstream oss;
	oss << _statusCode;
	return (oss.str());
}

std::string HttpResponse::getStatusMessage() const
{
	return (_statusMessage);
}

////////////////////////////////////////////////////////////////////////////////
/// Utility functions

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	isFile(const std::string path)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISREG(buffer.st_mode))
		return (true);
	return (false);
}

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	isDir(const std::string path)
{
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISDIR(buffer.st_mode))
		return (true);
	return (false);
}

/// @brief Get the body length of a string.
/// @param body 
/// @return `std::string`
std::string	getcontentLength(const std::string& body)
{
	if (body.empty())
		return ("");
	std::ostringstream	oss;
	oss << body.size();
	return (oss.str());
}