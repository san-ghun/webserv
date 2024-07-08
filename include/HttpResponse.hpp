/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/08 22:27:34 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

enum e_status_code
{
	STATUS_SUCCESS = 200,
	STATUS_CREATED = 201,
	STATUS_NO_CONTENT = 204,
	STATUS_MOVED = 301,
	STATUS_BAD_REQUEST = 400,
	STATUS_FORBIDDEN = 403,
	STATUS_NOT_FOUND = 404,
	STATUS_NOT_ALLOWED = 405,
	STATUS_CONFLICT = 409,
	STATUS_LENGTH_REQUIRED = 411,
	STATUS_TOO_LARGE = 413,
	STATUS_URI_TOO_LONG = 414,
	STATUS_INTERNAL_ERR = 500,
	STATUS_NOT_IMPLEMENTED = 501
};

class	HttpResponse
{
	public:
		HttpResponse();
		~HttpResponse();

		void								setStatusCode(int code, const std::string statusMessage);
		void								setHeader(const std::string key, const std::string value);
		void								setBody(const std::string bodyContent);
		
		std::string							toString() const;

		static HttpResponse					fromFile(const std::string filePath);
		static HttpResponse					badRequest_400();
		static HttpResponse					forbidden_403();
		static HttpResponse					notFound_404();
		static HttpResponse					methodNotAllowed_405();
		static HttpResponse					requestTimeout_408();
		static HttpResponse					requestEntityTooLarge_413();
		static HttpResponse					imaTeapot_418();
		static HttpResponse					internalServerError_500();

	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::string							_body;
		
		std::map<std::string, std::string>	_headers;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
};

#endif