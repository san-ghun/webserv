/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 21:29:54 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpResponse.hpp"

// TODO add doc

HttpResponse::HttpResponse()
	: _statusCode(200), _statusMessage("OK")
{
}

HttpResponse::~HttpResponse()
{
}

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
	std::cout << "TEST | HttpResponse::toString" <<  std::endl;
	std::cout << _getResponseLine() + _getHeadersString() + "\r\n\r\n" + _body << std::endl;
	return (_getResponseLine() + _getHeadersString() + "\r\n\r\n" + _body);
}

std::string	getDefualtPagePath(int page_code)
{
	std::ostringstream oss;
	oss << "./www/error_pages/" << page_code << ".html";
	return (oss.str());
}

/// @brief Creates an HttpResponse object by reading the contents of a file.
/// 
/// @param file_path The path to the file to be read.
/// @return HttpResponse The created HttpResponse object.
HttpResponse	HttpResponse::fromFile(const std::string file_path)
{
	HttpResponse	resp;
	std::ifstream	file(file_path.data());

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
/// @brief static functions to create HttpResponse objects
/// 	   with specific status codes.
/// @return HttpResponse The created HttpResponse object.
///
// TODO think about seperate functions to another class.
////////////////////////////////////////////////////////////////////////////////




HttpResponse	HttpResponse::badRequest_400()
{
	HttpResponse	resp;
	resp.setStatusCode(400, "Bad Request");
	resp.fromFile(getDefualtPagePath(400));
	return (resp);
}

HttpResponse	HttpResponse::forbidden_403()
{
	HttpResponse	resp;
	resp.setStatusCode(403, "Forbidden");
	resp.fromFile(getDefualtPagePath(403));
	return (resp);
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
	resp.setStatusCode(404, "Not Found");
	std::string		defaultPagePath = getDefualtPagePath(404);
	
	HttpResponse	fileResponse = fromFile(defaultPagePath);
	if (fileResponse.getBody().empty())
		resp.setBody("<html><body><h1>404 Not Found</h1></body></html>");
	else
		resp.setBody(fileResponse.getBody());
	return (resp);
}

HttpResponse	HttpResponse::methodNotAllowed_405()
{
	HttpResponse	resp;
	resp.setStatusCode(405, "Method Not Allowed");
	resp.fromFile(getDefualtPagePath(405));
	return (resp);
}

HttpResponse	HttpResponse::requestTimeout_408()
{
	HttpResponse	resp;
	resp.setStatusCode(408, "Request Timeout");
	resp.fromFile(getDefualtPagePath(408));
	return (resp);
}

HttpResponse	HttpResponse::requestEntityTooLarge_413()
{
	HttpResponse	resp;
	resp.setStatusCode(413, "Request Entity Too Large");
	resp.fromFile(getDefualtPagePath(413));
	return (resp);
}

HttpResponse	HttpResponse::imaTeapot_418()
{
	HttpResponse resp;
	resp.setStatusCode(418, "I'm a Teapot");
	resp.fromFile(getDefualtPagePath(418));
	return (resp);
}

HttpResponse	HttpResponse::internalServerError_500()
{
	HttpResponse resp;
	resp.setStatusCode(500, "Internal Server Error");
	resp.fromFile(getDefualtPagePath(500));

	return (resp);
}

HttpResponse	HttpResponse::success_200()
{
	HttpResponse resp;
	resp.setStatusCode(200, "OK");
	return (resp);
}

HttpResponse	HttpResponse::notImplemented_501()
{
	HttpResponse resp;
	resp.setStatusCode(501, "Not Implemented");
	resp.setBody("<html><body><h1>501 Not Implemented</h1></body></html>");
	return (resp);
}