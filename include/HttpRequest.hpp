/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/11 20:02:56 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>

# define WHITESPACE	" \t\r\n"

class	HttpRequest
{
	public:
		HttpRequest();
		~HttpRequest();

		bool								parse(const std::string& requestData);

		std::string							getMethod() const;
		std::string							getUri() const;
		std::string							getVersion() const;

		std::map<std::string, std::string>	getHeaders() const;
		std::string							getBody() const;

		bool								isConnectionClose() const;
		static std::string					trim(const std::string& str);
	private:
		std::string							_method;
		std::string							_uri;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		bool								_parseRequestLine(const std::string requestLine);
		bool								_parseHeaders(const std::string headerLines);
		bool								_parseBody(const std::string bodylines);
};

#endif