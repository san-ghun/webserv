/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:01 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>
# include <vector>

class HttpResponse;

# define WHITESPACE	" \t\r\n"

struct t_readed_parts {
    std::string					request;
    std::vector<std	::string>	headers;
    std::string					body;
	bool						iscomplete;
	t_readed_parts() : iscomplete(false) {}
};

enum e_uri_type {
	DEFAULT = 0,
	PATH,
	DIRECTORT
};

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

		void								setUri(std::string uri);

		bool								isConnectionClose() const;
		static std::string					trim(const std::string& str);
	private:
		std::string							_method;
		std::string							_uri;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		t_readed_parts						_splitRequestData(const std::string& requestData);
		bool								_parseRequestLine(const std::string requestLine);
		bool								_parseHeaders(const std::vector<std	::string> headerLines);
		bool								_parseBody(const std::string bodylines);
		std::vector<std	::string>			_convertPartToHeaders(std::istringstream& iss);
		std::string 						_convertPartToBody(std::istringstream& iss);

};

#endif