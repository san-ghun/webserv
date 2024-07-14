/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/14 22:26:03 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

std::string							getErrorPagePath(int pageCode);

class	HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(const std::string filePath);
		HttpResponse(const HttpResponse& other);
		HttpResponse& operator=(const HttpResponse& other);
		~HttpResponse();
		
		void								setStatusCode(int code);
		void								setStatusCode(int code, const std::string statusMessage);
		void								setHeader(const std::string key, const std::string value);
		void								setBody(const std::string bodyContent);
		
		std::string							getBody();
		std::string							toString() const;

		void								fromFile(const std::string filePath);
		// static HttpResponse					fromFile(const std::string filePath);
		static HttpResponse					badRequest_400();
		static HttpResponse					forbidden_403();
		static HttpResponse					notFound_404();
		static HttpResponse					methodNotAllowed_405();
		static HttpResponse					requestTimeout_408();
		static HttpResponse					requestEntityTooLarge_413();
		static HttpResponse					imaTeapot_418();
		static HttpResponse					internalServerError_500();
		static HttpResponse					success_200();
		static HttpResponse					notImplemented_501();

	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::string							_body;
		std::map<std::string, std::string>	_headers;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
		std::string							_getResponseLine() const;

		static const std::map<int, std::string>&	_getStatusMessages();
		static HttpResponse							_errorResponse(int code);
};

#endif