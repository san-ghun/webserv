/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/21 19:40:33 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <map>
# include <vector>

# include "Util.hpp"
# include "RequestParser.hpp"

class Body;
class BodyContainer;
class HttpResponse;


# define WHITESPACE	" \t\r\n"
# define NOT_SET	-1

struct ReadedLines {
    std::string					request;
    std::vector<std::string>	headers;
    std::vector<std::string>	bodyLines;
};


class	HttpRequest
{
	public:
		HttpRequest();
		HttpRequest(std::string& data);

		~HttpRequest();

		bool								parse(const std::string& requestData);

		std::string							getMethod() const;
		std::string							getUri() const;
		std::string							getVersion() const;

		std::map<std::string, std::string>	getHeaders() const;
		BodyContainer						getBody() const;
		ssize_t								getContentLength() const;
		bool								getIsParsed() const;

		void								setUri(const std::string& uri);
		void								setMethod(const std::string& method);
		void								setVersion(const std::string& version);
		void								setHeaders(const std::map<std::string, std::string>& headers);
		void								setBody(const struct Body& body);
		void								setContentLength(const ssize_t& contentLength);

		bool								isConnectionClose() const;

		static std::string					trim(const std::string& str);
	private:
		std::string							_method;			// GET, POST, DELETE
		std::string							_uri;				//
		std::string							_version;			// HTTP/1.1
		std::map<std::string, std::string>	_headers;			// key: value
		BodyContainer						_body;				// raw, chunked, formdata
		ssize_t								_contentLength;		// from Headers["Content-Length"], if not found NOT_SET -1
		bool								_isParsed;			// from `parse` method

		bool								_parseGET();
		bool								_parsePOST(const std::vector<std::string>& bodyLines);
		bool								_parseDELETE(const std::vector<std::string>& bodyLines);

		// Chunked
		bool								_parseChunkedBody(const std::vector<std::string>& bodyLines);
		size_t								_extractChunkSize(const std::string& line);
		std::string							_readChunkData(std::istringstream* stream, size_t chunkSize);
		void								_consumeCRLF(std::istringstream* stream);
		void								_addEndOfChunks();
		void								_addChunkSegment(const std::string& data, size_t size);

		// FormData
		bool								_parseFormDataBody(const std::vector<std::string>& bodyLines);
		bool								_processFormData(const std::vector<std::string>& bodyLines);
		std::string							_extractBoundary(const std::string& contentType) const;;
		FormDataEntry						_entryWithHeaders(std::istringstream* stream);
		bool								_addFormDataEntry(std::istringstream* stream);

		bool								_parseBodySpecificMethod(const std::vector<std::string>& bodyLines);
		ReadedLines							_splitRequestData(const std::string& requestData);
		bool								_parseRequestLine(const std::string& requestLine);
		bool								_parseHeaders(const std::vector<std	::string>& headerLines);

		std::vector<std	::string>			_convertPartToHeaders(std::istringstream& iss);
		std::vector<std	::string>			_convertPartToBody(std::istringstream& iss);



};

#endif