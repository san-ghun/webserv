/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 15:02:09 by minakim          ###   ########.fr       */
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

// TODO  How will the return value be used in other functions?
//			- check Server.cpp
std::string	HttpResponse::toString() const
{
	return (_getHeadersString() + "\r\n\r\n" + _body);
}

/// TODO test: function, check if it works (internalServerError_500())
/// @brief Creates an HttpResponse object by reading the contents of a file.
/// 
/// @param filePath The path to the file to be read.
/// @return HttpResponse The created HttpResponse object.
HttpResponse	HttpResponse::fromFile(const std::string filePath)
{
	HttpResponse	resp;
	std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);

	if (!file.is_open())
		return (notFound_404());
	std::streamsize	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(fileSize);
	if (!file.read(buffer.data(), fileSize))
		return (file.close(), internalServerError_500());
	resp.setBody(std::string(buffer.data(), fileSize));
	file.close();
	return (resp);
}

////////////////////////////////////////////////////////////////////////////////

/// std::string	HttpResponse::_getHeadersString() const
/// std::string HttpResponse::_getStatusLine() const
// TODO log design, doc

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

std::string HttpResponse::_getStatusLine() const
{
	std::stringstream statusLine;
	statusLine << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	return (statusLine.str());
}

////////////////////////////////////////////////////////////////////////////////

/// @brief static functions to create HttpResponse objects with specific status codes.
/// @return HttpResponse The created HttpResponse object.
///
// TODO think about seperate functions to another class.

HttpResponse	HttpResponse::badRequest_400()
{
	HttpResponse	resp;
	resp.setStatusCode(400, "Bad Request");
	return (resp);
}

HttpResponse	HttpResponse::forbidden_403()
{
	HttpResponse	resp;
	resp.setStatusCode(403, "Forbidden");
	return (resp);
}

HttpResponse	HttpResponse::notFound_404()
{
	HttpResponse	resp;
	resp.setStatusCode(404, "Not Found");
	return (resp);
}

HttpResponse	HttpResponse::methodNotAllowed_405()
{
	HttpResponse	resp;
	resp.setStatusCode(405, "Method Not Allowed");
	return (resp);
}

HttpResponse	HttpResponse::requestTimeout_408()
{
	HttpResponse	resp;
	resp.setStatusCode(408, "Request Timeout");
	return (resp);
}

HttpResponse	HttpResponse::requestEntityTooLarge_413()
{
	HttpResponse	resp;
	resp.setStatusCode(413, "Request Entity Too Large");
	return (resp);
}

HttpResponse	HttpResponse::imaTeapot_418()
{
	HttpResponse resp;
	resp.setStatusCode(418, "I'm a Teapot");
	return (resp);
}

HttpResponse	HttpResponse::internalServerError_500()
{
	HttpResponse resp;
	resp.setStatusCode(500, "Internal Server Error");
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
	return (resp);
}