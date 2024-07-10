/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/10 10:21:47 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpRequest.hpp"

/// @todo check for necessary initialization
HttpRequest::HttpRequest()
{}

HttpRequest::~HttpRequest()
{}


/// @todo check the `requestData` formats
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


/////////////////////////////////////////////////////////////////////////////////////

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



/////////////////////////////////////////////////////////////////////////////////////
/// @todo Update all parseXXX functions to include new line parsing logic.
///       - Verify the trim logic used in this function.
///       - Ensure future param values (parse memberfunction == key) are handled.

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


# define WHITESPACE	" \t"			// [?] add /r/n
/// @todo check the trim logic in this function
std::string							HttpRequest::_trim(const std::string& str)
{
    size_t first = str.find_first_not_of(WHITESPACE);
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(WHITESPACE);
    return (str.substr(first, last - first + 1));
}