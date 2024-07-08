/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/08 22:55:17 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{}

HttpRequest::~HttpRequest()
{}

bool								HttpRequest::parse(const std::string requestData)
{
	std::istringstream iss(requestData);
	std::string requestLine;
	std::string headerLines;
	std::string bodylines;
	
	iss >> requestLine;
	_parseRequestLine(requestLine);
	iss >> headerLines;
	_parseHeaders(headerLines);
	iss >> bodylines;
	_parseBody(bodylines);
	return (true);
}

std::string							HttpRequest::getMethod() const
{
	return (_method);
}

std::string							HttpRequest::getPath() const
{
	return (_path);
}

std::string							HttpRequest::getVersion() const
{
	return (_version);
}

std::map<std::string, std::string>	HttpRequest::getHeaders() const
{
	return (_headers);
}

std::string							HttpRequest::getBody() const
{
	if (_body.empty())
		return ("");
	return (_body);
}

bool								HttpRequest::isConnectionClose() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Connection");
	if (it != _headers.end() && it->second == "close")
		return (true);
	return (false);
}


/**
std::string							_method
std::string							_path
std::string							_version
std::map<std::string, std::string>	_headers
std::string							_body
 */

void								HttpRequest::_parseRequestLine(const std::string requestLine)
{
	std::string trimmedLine = _trim(requestLine);
	std::istringstream iss(trimmedLine);
	iss >> _method >> _path >> _version;
}

void								HttpRequest::_parseHeaders(const std::string headerLines)
{}

void								HttpRequest::_parseBody(const std::string bodylines)
{}

std::string							HttpRequest::_trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}