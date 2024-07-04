/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 10:56:05 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>

class	HTTPResponse
{
	public:
		HTTPResponse();
		~HTTPResponse();

		void								setStatusCode(int code, const std::string statusMessage);
		void								setHeader(const std::string key, const std::string value);
		void								setBody(const std::string bodyContent);

		std::string							toString() const;

		static HTTPResponse					fromFile(const std::string filePath);
		static HTTPResponse					badRequest_400();
		static HTTPResponse					forbidden_403();
		static HTTPResponse					notFound_404();
		static HTTPResponse					methodNotAllowed_405();
		static HTTPResponse					requestTimeout_408();
		static HTTPResponse					requestEntityTooLarge_413();
		static HTTPResponse					imaTeapot_418();
		static HTTPResponse					internalServerError_500();

	private:
		int									_statusCode;
		std::string							_statusMessage;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		std::string							_getStatusLine() const;
		std::string							_getHeadersString() const;
};

#endif