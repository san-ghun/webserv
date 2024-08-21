/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/21 19:52:30 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HttpRequest.hpp"
#include "RequestParser.hpp"

HttpRequest::HttpRequest()
	: _contentLength(NOT_SET),  _isParsed(false)
{}

HttpRequest::HttpRequest(std::string& data)
	: _contentLength(NOT_SET)
{
	_isParsed = parse(data);
}

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
	ReadedLines separatedData = _splitRequestData(requestData);
	if (!_parseRequestLine(separatedData.request))
		return (false);
	if (!_parseHeaders(separatedData.headers))
		return (false);
	if (!_parseBodySpecificMethod(separatedData.bodyLines))
		return (false);
	return (true);
}

bool	HttpRequest::_parseBodySpecificMethod(const std::vector<std::string>& bodyLines)
{
	if (_method == "GET")
		return (_parseGET());
	else if (_method == "POST")
		return (_parsePOST(bodyLines));
	else if (_method == "DELETE")
		return (_parseDELETE(bodyLines));
	return (true);
}

////////////////////////////////////////////////////////////////////////////////
/// Parsing functions: GET
////////////////////////////////////////////////////////////////////////////////

bool	HttpRequest::_parseGET()
{
	_body.clear();
	return (true);
}

////////////////////////////////////////////////////////////////////////////////
/// Parsing functions: POST
////////////////////////////////////////////////////////////////////////////////

bool	HttpRequest::_parsePOST(const std::vector<std::string>& bodyLines)
{
	if (_contentLength <= 0)
		return (false);
	if (_headers["Content-Type"] == "application/json" || _headers["Content-Type"] == "text/plain") {
		_body.initRaw();
		_body.getRaw()->setData(toString(bodyLines).substr(0, _contentLength));
	}
	else if (_headers["Transfer-Encoding"] == "chunked")
	{
		_body.initChunked();
		if (!_parseChunkedBody(bodyLines))
			return (false);
	}
	else if (_headers["Content-Type"].find("multipart/form-data") != std::string::npos)
	{
		std::string	boundary = _extractBoundary(_headers["Content-Type"]);
		if (boundary.empty())
			return (false);
		_body.initFormData(boundary);
		if (!_parseFormDataBody(bodyLines))
			return (false);
	}
	return (true);
}

////////////////////////////////////////////////////////////////////////////////

/* /// @example POST request with chunked body
POST /upload HTTP/1.1
Host: example.com
Transfer-Encoding: chunked
Content-Type: text/plain

7\r\n
Mozilla\r\n
9\r\n
Developer\r\n
7\r\n
Network\r\n
0\r\n
\r\n
*/

bool HttpRequest::_parseChunkedBody(const std::vector<std::string>& bodyLines)
{
	std::istringstream stream(bodyLines[0]);
	std::string line;

	while (std::getline(stream, line))
	{
		if (line.empty())
			continue;
		size_t	chunkSize = _extractChunkSize(line);
		if (chunkSize == 0)
		{
			_addEndOfChunks();
			break;
		}
		std::string chunkData = _readChunkData(&stream, chunkSize);
		_addChunkSegment(chunkData, chunkSize);
	}
	return (true);
}

size_t	HttpRequest::_extractChunkSize(const std::string& line)
{
	try
	{
		return std::strtol(removeCRLF(line).c_str(), 0, 16);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("Invalid chunk size format");
	}
}

std::string HttpRequest::_readChunkData(std::istringstream* stream, size_t chunkSize)
{
	std::string chunkData(chunkSize, '\0');
	stream->read(&chunkData[0], chunkSize);
	_consumeCRLF(stream);
	return (chunkData);
}

void	HttpRequest::_consumeCRLF(std::istringstream* stream)
{
	std::string	crlf;
	std::getline(*stream, crlf);
}

void	HttpRequest::_addEndOfChunks()
{
	ChunkSegment	segment("", 0);
	segment.setEOF();
	_body.getChunked()->addChunkSegment(segment);
}

void	HttpRequest::_addChunkSegment(const std::string& data, size_t size)
{
	ChunkSegment	segment(data, size);
	_body.getChunked()->addChunkSegment(segment);
}


////////////////////////////////////////////////////////////////////////////////

/* @example POST request with multipart/form-data
POST /upload HTTP/1.1
Host: example.com
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW

------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="text"

Hello, World!
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="file"; filename="example.txt"
Content-Type: text/plain

(File content here)
------WebKitFormBoundary7MA4YWxkTrZu0gW--
*/


bool	HttpRequest::_parseFormDataBody(const std::vector<std::string>& bodyLines)
{
	_body.getFormData()->setBoundary(_extractBoundary(_headers["Content-Type"]));
	if (_body.getFormData()->getBoundary().empty())
		return (false);
	return (_processFormData(bodyLines));
}

std::string	HttpRequest::_extractBoundary(const std::string& contentType) const
{
	size_t	pos = contentType.find("boundary=");
	if (pos == std::string::npos)
		return ("");
	return (contentType.substr(pos + 9));
}

bool	HttpRequest::_processFormData(const std::vector<std::string>& bodyLines)
{
	std::istringstream	stream(bodyLines[0]);
	std::string			line;

	while (std::getline(stream, line))
	{
		if (line.empty())
			continue;
		if (line == "--" + _body.getFormData()->getBoundary())
		{
			if (!_addFormDataEntry(&stream))
				return (false);
		}
		else if (line == "--" + _body.getFormData()->getBoundary() + "--")
			return (true);
	}
	return (false);
}

bool	HttpRequest::_addFormDataEntry(std::istringstream* stream)
{
	FormDataEntry				entry(_entryWithHeaders(stream));
	std::string					line;
	std::stringstream			content;

	while (std::getline(*stream, line))
	{
		if (line != "--" + _body.getFormData()->getBoundary() && line != "--" + _body.getFormData()->getBoundary() + "--")
			content << line << "\n";
		stream->seekg(-static_cast<int>(line.length() + 1), std::ios_base::cur);
		entry.setData(content.str());
		entry.setProcessed();
		_body.getFormData()->addFormDataEntry(entry);
		return (true);
	}
	return (false);
}

FormDataEntry	HttpRequest::_entryWithHeaders(std::istringstream* stream)
{
	FormDataEntry	entry;
	std::string		line;

	while (std::getline(*stream, line))
	{
		if (line.empty())
			break;
		if (line.find("Content-Disposition:") == 0)
			entry.handleContentDisposition(line);
		else if (line.find("Content-Type:") == 0)
			entry.handleContentType(line);
		// else if (line.find("Content-Transfer-Encoding:") == 0)
		// 	entry.handleContentTransferEncoding(line);
	}
	return (entry);
}

////////////////////////////////////////////////////////////////////////////////
/// Parsing functions: DELETE
////////////////////////////////////////////////////////////////////////////////

bool	HttpRequest::_parseDELETE(const std::vector<std::string>& bodyLines)
{
	// TODO: implement DELETE parsing logic
	(void)bodyLines;
	return (true);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Separate the request data into request line, headers, and body.
/// @param requestData The request data to be separated.
/// @return A struct containing the separated request data.
ReadedLines HttpRequest::_splitRequestData(const std::string& requestData)
{
	ReadedLines			data;
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
	data.bodyLines = _convertPartToBody(iss);
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

std::vector<std::string> HttpRequest::_convertPartToBody(std::istringstream& iss)
{
	std::string					readline;
	std::vector<std::string>	drafts;

	while (std::getline(iss, readline))
		drafts.push_back(readline);
	// if (_contentLength > 0 && drafts.size() > _contentLength)
	// 	drafts = drafts.substr(0, _contentLength);
		// TODO: implement body.length() != _contecntLength occurence Reqeust-Error
		// ** BUT not here
	return (drafts);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Parses the request line and extracts the method, path, and version.
/// @param requestLine `_method` `_uri` `_version`, example: GET /path/resource HTTP/1.1
/// @return bool
bool HttpRequest::_parseRequestLine(const std::string& requestLine)
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
bool HttpRequest::_parseHeaders(const std::vector<std::string> &headerLines)
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
	if (_headers.find("Content-Length") != _headers.end())
		_contentLength = toSizeT(_headers["Content-Length"]);
	return (true);
}

////////////////////////////////////////////////////////////////////////////////
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
/// Checker functions
////////////////////////////////////////////////////////////////////////////////

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

BodyContainer	HttpRequest::getBody() const
{
	return (_body);
}

ssize_t	HttpRequest::getContentLength() const
{
	return (_contentLength);
}

bool	HttpRequest::getIsParsed() const
{
	return (_isParsed);
}

////////////////////////////////////////////////////////////////////////////////
/// Setters
////////////////////////////////////////////////////////////////////////////////

void	HttpRequest::setUri(const std::string& uri)
{
	_uri = uri;
}

void	HttpRequest::setMethod(const std::string& method)
{
	_method = method;
}

void	HttpRequest::setVersion(const std::string& version)
{
	_version = version;
}

void	HttpRequest::setHeaders(const std::map<std::string, std::string>& headers)
{
	_headers = headers;
}

void	HttpRequest::setBody(const struct Body& body)
{
	_body = body;
}

void	HttpRequest::setContentLength(const ssize_t& contentLength)
{
	_contentLength = contentLength;
}