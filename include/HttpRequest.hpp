/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 19:17:46 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>

# define WHITESPACE	" \t\r\n"

typedef struct s_read_request {
    std::string					request;
    std::vector<std	::string>	headers;
    std::string	body;
	bool						iscomplete;

}	t_read_request;

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
		bool								_parseHeaders(const std::vector<std	::string> headerLines);
		bool								_parseBody(const std::string bodylines);
		t_read_request						_separateRequestData(const std::string& requestData);
		std::vector<std	::string>			_dataToHeaders(std::istringstream& iss);
		std::string 						_dataToBody(std::istringstream& iss);

};

#endif