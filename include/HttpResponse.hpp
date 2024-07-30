/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/25 16:46:01 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

# include "Config.hpp"

# define LOCATION_ROOT_PATH "/www"

class HttpRequest;
class Config;

struct t_page_detail
{
	std::string		path;
	bool 			isValid;
};

std::string		getFullErrorPath(const std::string& path);
bool			isFile(const std::string path);
bool			isDir(const std::string path);
t_page_detail	constructPageDetail(const std::string& path);


class	HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(const std::string& filePath);
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);
		~HttpResponse();
		
		void					setStatusCode(int code);
		void					setStatusCode(int code, const std::string statusMessage);
		void					setHeader(const std::string key, const std::string value);
		void					setBody(const std::string bodyContent);
		void					setDefaultHeaders();
		static void				setDefaultHeaders(HttpResponse& resp);

		std::string				getBody();
		size_t					getBodyLength();
		std::string				getResponseLine() const;
		std::string				toString() const;
		int						getStatusCode() const;
		std::string				getStatusMessage() const;
		std::string				toString(int value) const;
		std::string				toString(size_t value) const;
		void					initializefromFile(const std::string& filePath);

		static HttpResponse		badRequest_400();
		static HttpResponse		forbidden_403();
		static HttpResponse		notFound_404();
		static HttpResponse		methodNotAllowed_405();
		static HttpResponse		requestTimeout_408();
		static HttpResponse		requestEntityTooLarge_413();
		static HttpResponse		imaTeapot_418();
		static HttpResponse		internalServerError_500();
		static HttpResponse		success_200();
		static HttpResponse		notImplemented_501();

	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::string							_body;
		std::map<std::string, std::string>	_headers;
		size_t								_bodyLength;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
		void								_fileToBody(const std::string& filePath);

		static HttpResponse					_createErrorResponse(int code);
		static HttpResponse					_createSimpleHttpResponse(int code);

		std::string							_generateHtmlBody();
		void								_setDefaultHeadersImpl();

		static const std::map<int, std::string>&	_StaticInitStatusMap();
};

#endif