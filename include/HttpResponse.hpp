/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:07 by minakim          ###   ########.fr       */
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

std::string	getFullErrorPath(int pageCode);
bool		isFile(const std::string path);
bool		isDir(const std::string path);

struct t_page_detail
{
	std::string		path;
	bool 			isValid;
};


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
		std::string				getResponseLine() const;
		std::string				getStatusCode() const;
		std::string				getStatusMessage() const;
		std::string				toString() const;
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

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
		std::string							_fileToString(const std::string& filePath);

		static HttpResponse					_createErrorResponse(int code);
		static HttpResponse					_createSimpleHttpResponse(int code);

		std::string							_generateHtmlBody();
		void								_setDefaultHeadersImpl();

		static const std::map<int, std::string>&	_StaticInitStatusMap();
};

/// Utility functions
std::string							getcontentLength(const std::string& body);

#endif