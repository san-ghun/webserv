/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/10/23 11:19:30 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>
# include <vector>

# include "Util.hpp"

class HttpResponse;


# define WHITESPACE	" \t\r\n"
# define NOT_SET	0

struct ReadedLines
{
    std::string					request;
    std::vector<std::string>	headers;
    std::string	bodyLines;
};


class	HttpRequest
{
public:
	enum e_body_type
	{
		RAW,
		CHUNKED,
		FORM_DATA,
		NONE
	};

public:

	HttpRequest();
	HttpRequest(std::string& data);
	~HttpRequest();
	bool								parse(const std::string& requestData);
	
	std::string							getMethod() const;
	std::string							getUri() const;
	std::string							getVersion() const;
	std::map<std::string, std::string>	getHeaders() const;
	std::string							getBody() const;
	
	size_t								getContentLength() const;
	
	void								setUri(const std::string& uri);
	void								setMethod(const std::string& method);
	void								setVersion(const std::string& version);
	void								setHeaders(const std::map<std::string, std::string>& headers);
	void								setBody(const std::vector<std::string>& bodyLines, e_body_type type);
	void								setBody(const std::string& bodyLines, e_body_type type);
	void								setContentLength(const ssize_t& contentLength);
	
	bool								hasBody() const;

	bool								isConnectionClose() const;
	static std::string					trim(const std::string& str);
	
private:
	std::string							_method;			// GET, POST, DELETE
	std::string							_uri;				//
	std::string							_version;			// HTTP/1.1
	std::map<std::string, std::string>	_headers;			// key: value
	std::string							_body;				// raw, chunked, formdata
	e_body_type							_type;				// type of body @see e_body_type
	std::pair<bool, size_t>				_content;		// from Headers["Content-Length"], if not found NOT_SET -1

	
	ReadedLines							_splitRequestData(const std::string& requestData);
	
	bool								_processRequestBody(const std::string& bodyLines);
	bool								_parseRequestLine(const std::string& requestLine);
	bool								_parseHeaders(const std::vector<std	::string>& headerLines);
	
	std::vector<std	::string>			_convertPartToHeaders(std::istringstream& iss);
	std	::string						_convertPartToBodyLines(std::istringstream& iss);

};

// TODO: implement "<< operator" for HttpRequest
// std::ostream& operator<<(std::ostream& os, const HttpRequest& request);
#endif