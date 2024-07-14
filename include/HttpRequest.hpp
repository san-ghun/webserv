/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/14 14:49:37 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>

# define WHITESPACE	" \t\r\n"


struct t_read_request {
    std::string					request;
    std::vector<std	::string>	headers;
    std::string	body;
	bool						iscomplete;
	t_read_request() : iscomplete(false) {}
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

		bool								isConnectionClose() const;
		static std::string					trim(const std::string& str);
	private:
		std::string							_method;
		std::string							_uri;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;

		t_read_request						_splitRequestData(const std::string& requestData);
		bool								_parseRequestLine(const std::string requestLine);
		bool								_parseHeaders(const std::vector<std	::string> headerLines);
		bool								_parseBody(const std::string bodylines);
		std::vector<std	::string>			_dataToHeaders(std::istringstream& iss);
		std::string 						_dataToBody(std::istringstream& iss);

};

#endif