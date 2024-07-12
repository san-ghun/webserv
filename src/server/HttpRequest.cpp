/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 14:44:09 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpRequest.hpp"

/// TODO check for necessary initialization
HttpRequest::HttpRequest()
{}

HttpRequest::~HttpRequest()
{}

////////////////////////////////////////////////////////////////////////////////

std::string	HttpRequest::getMethod() const
{
	return (_method);
}

std::string	HttpRequest::getUri() const
{
	return (_uri);
}

std::string	HttpRequest::getVersion() const
{
	return (_version);
}

std::map<std::string, std::string>	HttpRequest::getHeaders() const
{
	return (_headers);
}

std::string	HttpRequest::getBody() const
{
	if (_body.empty())
		return ("");
	return (_body);
}

bool HttpRequest::isConnectionClose() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Connection");
	if (it != _headers.end() && it->second == "close")
		return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////


/// @brief This function parses the request data and extracts
///			the method, path, version, headers, and body.
/// @param requestData The request data to be parsed. 
/// @return bool
bool HttpRequest::parse(const std::string& requestData)
{
	std::istringstream	iss(requestData);
	std::string			requestLine;
	std::string			headerLines;
	std::string			bodylines;

	if (requestData.empty())
		return (false);
	iss >> requestLine;
	iss >> headerLines;
	iss >> bodylines;
	if (!_parseRequestLine(requestLine))
		return (false);
	if (!_parseHeaders(headerLines))
		return (false);
	if (!_parseBody(bodylines))
		return (false);
	return (true);
}


/// @brief Parses the request line and extracts the method, path, and version.
/// @param requestLine `_method` `_uri` `_version`, example: GET /path/resource HTTP/1.1
/// @return bool
bool HttpRequest::_parseRequestLine(const std::string requestLine)
{
	std::string			trimmedLine = trim(requestLine);
	std::istringstream	iss(trimmedLine);

	if (requestLine.empty() || trimmedLine.empty())
		return (false);
	iss >> _method >> _uri >> _version;
	if (_method.empty() || _uri.empty() || _version.empty())
		return (false);
	return (true);
}

/// @brief Parses the header lines and extracts the headers.
/// @param headerLines key:value pairs separated by `\r\n`
/// @return bool
bool HttpRequest::_parseHeaders(const std::string headerLines)
{
	if (headerLines.empty())
		return (false);
	std::istringstream	iss(headerLines);
	std::string			line;

	while (std::getline(iss, line))
	{
		if (line.empty())
			continue;
		std::string	key = trim(line.substr(0, line.find(":")));
		if (key.empty())
			return (false);
		std::string	value = trim(line.substr(line.find(":") + 1));
		if (value.empty())
			return (false);
		_headers.insert(std::make_pair(key, value));
		line.clear();
	}
	return (true);
}

/// @brief Parses the body and extracts the body.
/// @param bodylines exameple key1=value1&key2=value2
/// @return bool
bool HttpRequest::_parseBody(const std::string bodylines)
{
	if (bodylines.empty())
		return (false);
	_body = bodylines;
	return (true);
}

/// @brief Trims the string by removing leading and trailing whitespace.
/// @details `WHITESPACE`: Whitespace includes: space, tab, carriage return, and newline.
/// @param str `const std::string&`, The string to be trimmed.
/// @return The trimmed string.
std::string HttpRequest::trim(const std::string& str)
{
	std::string::size_type first = str.find_first_not_of(WHITESPACE);
	if (first == std::string::npos)
		return ("");
	std::string::size_type last = str.find_last_not_of(WHITESPACE);
	if (last == std::string::npos)
		return ("");
	return (str.substr(first, last - first + 1));
}