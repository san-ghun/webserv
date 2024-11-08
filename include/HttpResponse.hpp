/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/11/08 12:44:44 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

# include "Config.hpp"
# include "Util.hpp"

class HttpRequest;
class Config;
class Location;
class ErrorResponse;
class Context;

struct t_page_detail
{
	std::string		path;
	bool 			isValid;
};

enum e_status {
	STATUS_INVALID = -1,
	STATUS_INFORMATIONAL,
	STATUS_SUCCESS,
	STATUS_REDIRECTION,
	STATUS_ERROR
};

bool	isFile(const std::string path);
bool	isDir(const std::string path);
// std::string		generateHtmlBody(int code, const std::string& message);

class	HttpResponse
{
	public:
		HttpResponse(const Context& context);
		HttpResponse(const Context& context, const std::string& filePath);
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
		std::string				generateResponseToString() const;
		int						getStatusCode() const;
		std::string				getStatusMessage() const;
		void					initializefromFile(const Context& context, const std::string& filePath);

		static HttpResponse		createErrorResponse(int code, const Context& context);
		static HttpResponse		badRequest_400(const Context& context);
		static HttpResponse		forbidden_403(const Context& context);
		static HttpResponse		notFound_404(const Context& context);
		static HttpResponse		methodNotAllowed_405(const Context& context);
		static HttpResponse		requestTimeout_408(const Context& context);
		static HttpResponse		requestEntityTooLarge_413(const Context& context);
		static HttpResponse		imaTeapot_418(const Context& context);
		static HttpResponse		internalServerError_500(const Context& context);
		static HttpResponse		notImplemented_501(const Context& context);
		static HttpResponse		success_200(const Context& context);


		static e_status			checkStatusRange(int code);

	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::string							_body;
		std::map<std::string, std::string>	_headers;
		size_t								_bodyLength;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
		void								_fileToBody( const Context& context, const std::string& filePath);
		std::string							_generateHtmlBody();
		void								_setDefaultHeadersImpl();

		static const std::map<int, std::string>&	_staticInitStatusMap();

	protected:
		Context&							_context;

		HttpResponse		createErrorResponse(int code);
		HttpResponse		createSimpleHttpResponse(int code);
		t_page_detail		constructPageDetail(const std::string& path);
};


// TODO: implement "<< operator" for HttpResponse
// std::ostream& operator<<(std::ostream& os, const HttpResponse& response);
#endif