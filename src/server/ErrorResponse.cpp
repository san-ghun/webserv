#include "ErrorResponse.hpp"
#include "Location.hpp"
#include "Context.hpp"
#include "Config.hpp"
#include <fstream>

////////////////////////////////////////////////////////////////////////////////

ErrorResponse::ErrorResponse(const Context& context)
	: HttpResponse(context)
{}

ErrorResponse::~ErrorResponse()
{}


////////////////////////////////////////////////////////////////////////////////
HttpResponse ErrorResponse::generateErrorResponse(int code)
{
	if (checkStatusRange(code) != STATUS_ERROR)
		throw std::runtime_error("Invalid error code: " + toString(code));

	t_page_detail pageData = _fetchPageData(code);

	if (pageData.path.empty() || !pageData.isValid)
		return (createSimpleHttpResponse(code));
	HttpResponse resp(_context, pageData.path);
	resp.setStatusCode(code);
	return (resp);
}

t_page_detail ErrorResponse::_fetchPageData(int code)
{
	std::map<int, t_page_detail>::iterator it = _pageCache.find(code);

	if (it == _pageCache.end())
	{
		std::string	fullPath = _getFullErrorPath(_getErrorPagePath(code));
		_pageCache[code] = constructPageDetail(fullPath);
	}
	return (_pageCache[code]);
}

std::string ErrorResponse::_getErrorPagePath(int code) const
{
	const std::map<int, std::string>& errorPages = _context.getServer().error_pages;
	std::map<int, std::string>::const_iterator it = errorPages.find(code);
	if (it != errorPages.end())
		return (it->second);
	return ("");
}

std::string ErrorResponse::_getFullErrorPath(const std::string& path) const
{
	if (path.empty())
		return ("");
	return (_context.getLocation().getRootPath() + path);
}
