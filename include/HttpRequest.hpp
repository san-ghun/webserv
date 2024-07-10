/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/08 22:47:33 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>

class	HttpRequest
{
	public:
		HttpRequest();
		~HttpRequest();

		bool								parse(const std::string requestData);

		std::string							getMethod() const;
		std::string							getPath() const;
		std::string							getVersion() const;

		std::map<std::string, std::string>	getHeaders() const;
		std::string							getBody() const;

		bool								isConnectionClose() const;

	private:
		std::string							_method;
		std::string							_path;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		void								_parseRequestLine(const std::string requestLine);
		void								_parseHeaders(const std::string headerLines);
		void								_parseBody(const std::string bodylines);

		static std::string					_trim(const std::string& str);
};

#endif