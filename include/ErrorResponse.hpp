#ifndef ERRORRESPONSE_HPP
#define ERRORRESPONSE_HPP

#include <string>
#include <map>
#include "Location.hpp"
#include "HttpResponse.hpp"

class Location;
class HttpResponse;
class Context;

struct t_page_detail;

class ErrorResponse : public HttpResponse {
public:
	ErrorResponse(const Context& context);
	~ErrorResponse();
	HttpResponse generateErrorResponse(int code);

private:
	std::map<int, t_page_detail>	_pageCache;
	

	std::string		_getErrorPagePath(int code) const;
	std::string 	_getFullErrorPath(const std::string& path) const;
	t_page_detail	_fetchPageData(int code);

};

#endif