/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 15:59:46 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

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
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
};

#endif