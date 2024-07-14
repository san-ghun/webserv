/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/14 17:35:57 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpResponse.hpp"

// TODO add doc

HttpResponse::HttpResponse()
	: _statusCode(200), _statusMessage("OK")
{
}

HttpResponse::HttpResponse(const std::string filePath)
	:_statusCode(200), _statusMessage("OK")
{
	fromFile(filePath);
}

HttpResponse::HttpResponse(const HttpResponse& other)
{
	_statusCode = other._statusCode;
    _statusMessage = other._statusMessage;
	_headers = other._headers;
	_body = other._body;

}

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

HttpResponse::~HttpResponse()
{
}

////////////////////////////////////////////////////////////////////////////////

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

std::string	HttpResponse::getBody()
{
	return (_body);
}

std::string HttpResponse::_getResponseLine() const
{
	std::ostringstream oss;
	oss << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	return (oss.str());
}

std::string	HttpResponse::toString() const
{
	return (_getResponseLine() + _getHeadersString() + "\r\n\r\n" + _body);
}

////////////////////////////////////////////////////////////////////////////////

/// @brief Creates an Static HttpResponse object by reading the contents of a file.
/// @param filePath The path to the file to be read.
/// @return HttpResponse The created HttpResponse object.
HttpResponse	HttpResponse::fromFile(const std::string filePath)
{
	HttpResponse	resp;
	std::ifstream	file(filePath.data());

	if (!file.is_open())
		return (notFound_404());
	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	if (fileSize <= 0)
		return internalServerError_500();
	 std::string fileContents(fileSize, '\0');
	if (!file.read(&fileContents[0], fileSize))
		return (file.close(), internalServerError_500());	
	file.close();
	std::stringstream ss;
	ss << fileSize;
	resp.setBody(fileContents);
	resp.setHeader("Content-Length", ss.str());
	resp.setHeader("Connection", "close");
	return (resp);
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


std::string	getErrorPagePath(int pageCode)
{
	std::ostringstream oss;
	oss << "./www/error_pages/" << pageCode << ".html";
	return (oss.str());
}

const std::map<int, std::string>& HttpResponse::_getStatusMessages()
{
	static std::map<int, std::string> statusMessages;

	if (statusMessages.empty())
	{
		statusMessages[200] = "OK";
		// statusMessages[201] = "Created";
		// statusMessages[204] = "No Content";
		// statusMessages[301] = "Moved Permanently";
		// statusMessages[302] = "Found";
		statusMessages[400] = "Bad Request";
		// statusMessages[401] = "Unauthorized";
		statusMessages[403] = "Forbidden";
		statusMessages[404] = "Not Found";
		statusMessages[405] = "Method Not Allowed";
		statusMessages[408] = "Request Timeout";
		statusMessages[413] = "Request Entity Too Large";
		statusMessages[418] = "I'm a Teapot";
		statusMessages[500] = "Internal Server Error";
		statusMessages[501] = "Not Implemented";
	}
	return (statusMessages);
}


void HttpResponse::setStatusCode(int code)
{
	_statusCode = code;
	const std::map<int, std::string>& statusMessages = _getStatusMessages();
	std::map<int, std::string>::const_iterator it = statusMessages.find(code);
	if (it != statusMessages.end())
		_statusMessage = it->second;
	else
		throw std::runtime_error("Unknown status code");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief static functions to create HttpResponse objects
/// 	   with specific status codes.
/// @return HttpResponse The created HttpResponse object.
///
// TODO think about seperate functions to another class.
////////////////////////////////////////////////////////////////////////////////

/// @brief Handle error responses.
/// @param code 
/// @return HttpResponse
HttpResponse HttpResponse::_errorResponse(int code)
{
	HttpResponse resp(fromFile(getErrorPagePath(code)));
	resp.setStatusCode(code);	
	return (resp);
}

HttpResponse	HttpResponse::badRequest_400()
{
	return (_errorResponse(400));
}

HttpResponse	HttpResponse::forbidden_403()
{
	return (_errorResponse(403));
}

/// @brief Creates an HttpResponse object for a 404 Not Found error.
/// 
/// This function attempts to read a default 404 error page from the
/// filesystem. If the page is not found or cannot be read, it falls
/// back to a simple HTML message indicating a 404 error.
/// 
/// @return HttpResponse The generated HttpResponse object for the 404 error.
HttpResponse	HttpResponse::notFound_404()
{
	HttpResponse	resp;
	HttpResponse	from404File(_errorResponse(404));

	if (from404File.getBody().empty())
		resp.setBody("<html><body><h1>404 Not Found</h1></body></html>");
	else
		resp.setBody(from404File.getBody());
	return (resp);
}

HttpResponse	HttpResponse::methodNotAllowed_405()
{
	return (_errorResponse(405));

}

HttpResponse	HttpResponse::requestTimeout_408()
{
	return (_errorResponse(408));
}

HttpResponse	HttpResponse::requestEntityTooLarge_413()
{
	return (_errorResponse(413));
}

HttpResponse	HttpResponse::imaTeapot_418()
{
	return (_errorResponse(418));
}

HttpResponse	HttpResponse::internalServerError_500()
{
	return (_errorResponse(500));
}

// TODO add doc, make new mtethid to set header & body
HttpResponse	HttpResponse::success_200()
{
	HttpResponse resp;
	resp.setStatusCode(200, "OK");
	std::string body = "<html><body><h1>Success</h1></body></html>";
	resp.setBody(body);

	std::ostringstream oss;
    oss << body.size();
	resp.setHeader("Content-Length",oss.str());
    resp.setHeader("Content-Type", "text/html");
    resp.setHeader("Connection", "close");
	return (resp);
}

HttpResponse	HttpResponse::notImplemented_501()
{
	HttpResponse resp;
	resp.setStatusCode(501, "Not Implemented");

	std::string body = "<html><body><h1>501 Not Implemented</h1></body></html>";
	resp.setBody(body);

	std::ostringstream oss;
    oss << body.size();
	resp.setHeader("Content-Length",oss.str());
    resp.setHeader("Content-Type", "text/html");
    resp.setHeader("Connection", "close");
	return (resp);
}