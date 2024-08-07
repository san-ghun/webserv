/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/25 16:51:55 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpRequest.hpp"


HttpRequest::HttpRequest()
{}

HttpRequest::~HttpRequest()
{}

////////////////////////////////////////////////////////////////////////////////
/// The current parse logic for my request is to first split the syntax into
/// request, header, and body lines, and then parse the split syntax separately.
/// I've separated the 'split' and 'parse' parts so that one function does one job. 
////////////////////////////////////////////////////////////////////////////////

/// @brief This function parses the request data and extracts
///			the method, path, version, headers, and body.
/// @param requestData The request data to be parsed. 
/// @return bool
bool HttpRequest::parse(const std::string& requestData)
{
	t_readed_parts separatedData = _splitRequestData(requestData);
	if (!separatedData.iscomplete)
		return (false);
	if (!_parseRequestLine(separatedData.request))
		return (false);
	if (!_parseHeaders(separatedData.headers))
		return (false);
	if (_method == "POST" && !_parseBody(separatedData.body))
		return (false);
	return (true);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Separate the request data into request line, headers, and body.
/// @param requestData The request data to be separated.
/// @return A struct containing the separated request data.
t_readed_parts HttpRequest::_splitRequestData(const std::string& requestData)
{
	t_readed_parts		data;
	std::istringstream	iss(requestData);
	std::string			readline;
	std::string			drafts;

		if (requestData.empty())
		return (data);
	if (!std::getline(iss, readline))
		return (data);
	data.request = readline;
	data.headers = _convertPartToHeaders(iss);
	if (data.headers.empty())
		return (data);
	data.body = _convertPartToBody(iss);
	data.iscomplete = true;
	return (data);
}

std::vector<std	::string> HttpRequest::_convertPartToHeaders(std::istringstream& iss)
{
	std::string					readline;
	std::vector<std	::string>	res;

	while (std::getline(iss, readline) && readline != "\r")
		res.push_back(readline);
	return (res);
}

std::string HttpRequest::_convertPartToBody(std::istringstream& iss)
{
	std::string			readline;
	std::string			drafts;

	while (std::getline(iss, readline))
		drafts += readline;
	if (drafts.empty())
		return ("");
	return (drafts);
}

////////////////////////////////////////////////////////////////////////////////
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
bool HttpRequest::_parseHeaders(const std::vector<std::string> headerLines)
{
	if (headerLines.empty())
		return false;
	for (std::vector<std::string>::const_iterator	it = headerLines.begin();
													it != headerLines.end();
													++it)
	{
		const std::string& line = *it;
		if (line.empty())
			continue;
		size_t colonPos = line.find(':');
		if (colonPos == std::string::npos)
			return (false);
		std::string key = trim(line.substr(0, colonPos));
		if (key.empty())
			return (false);
		std::string value = trim(line.substr(colonPos + 1));
		if (value.empty())
			return (false);
		_headers.insert(std::make_pair(key, value));
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

////////////////////////////////////////////////////////////////////////////////

// TODO: check for necessary initialization
bool HttpRequest::isConnectionClose() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Connection");
	if (it != _headers.end() && it->second == "close")
		return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////
/// Getters
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
////////////////////////////////////////////////////////////////////////////////
/// Setters
////////////////////////////////////////////////////////////////////////////////

void	HttpRequest::setUri(std::string uri)
{
	_uri = uri;
}
