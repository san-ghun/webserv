/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:43 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "Location.hpp"

RequestHandler::RequestHandler()
{}

RequestHandler::~RequestHandler()
{}

////////////////////////////////////////////////////////////////////////////////
/// @brief Processes the incoming HTTP request and returns an appropriate response.
///
/// This function handles an incoming HTTP request by first logging the request details. It then
/// locates the corresponding `Location` object based on the request URI. If no matching location is
/// found, it returns a `404 Not Found` response. If the HTTP method is not allowed for the located
/// `Location`, it returns a `405 Method Not Allowed` response. Otherwise, it delegates the request
/// processing to the appropriate handler based on the method (`GET`, `POST`, `DELETE`, etc.).
///
/// @param request The `HttpRequest` object representing the incoming request.
/// @return An `HttpResponse` object containing the result of processing the request. 
/// 		1. Returns `404 Not Found` if no location is found.
///			2. `405 Method Not Allowed` if the method is not allowed.
///			3. or the result of `_processRequest` if the method is valid and the location is found.
HttpResponse	RequestHandler::handleRequest(const HttpRequest& request)
{
	// FIXME: change to Logger
	std::cout << "\r" << request.getMethod() << " | " << request.getUri() << " | " <<
		request.getVersion() << std::endl;

	HttpResponse	reps;
	Location*		location = _findLocation(request.getUri());

	if (location == NULL)
		return (HttpResponse::notFound_404());
	if (!_isAllowedMethod(request.getMethod(), (*location).getAllowedMethods()))
		return (HttpResponse::methodNotAllowed_405());
	reps = _processRequest(request, (*location));
	return (reps);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Finds the `Location` object that matches the request URI.
///
/// This function extracts the path from the given URI and searches for the most specific
/// location in the `Config` instance that matches the extracted path. It uses the location map
/// from the `Config` to find the best matching `Location`. If a match is found, the corresponding
/// `Location` object is returned. If no match is found, `NULL` is returned.
///
/// @param uri The request URI to extract the path from and match against the location map.
/// @return A pointer to the matching `Location` object if a match is found; otherwise, `NULL`.
Location*	RequestHandler::_findLocation(const std::string& uri)
{
	Config&								config = Config::getInstance();
	std::map<std::string, Location*>	locations = config.getLocationMap();

	std::string	matchedLocation = _getMatchedLocation(_extractPathFromUri(uri), locations);
	if (!matchedLocation.empty())
		return (locations.at(matchedLocation));
	return (NULL);
}

/// @brief Finds the longest location path that matches the request URI.
///
/// This function extracts the path from the URI and searches for the most specific location
/// that matches this path. It starts with the full path and progressively checks shorter
/// paths up to the root, using the provided locations map from the `Config` instance.
///
/// @param path The path extracted from the URI to match against the locations.
/// @param locations A map of location paths to `Location` objects from the `Config` instance.
/// @return The longest matching `location path`. If no match is found, returns an `empty string`.
std::string _getMatchedLocation(std::string path, const std::map<std::string, Location*>& locations)
{
	std::string	matched;

	while (!path.empty())
	{
		std::map<std::string, Location*>::const_iterator it = locations.find(path);
		if (it != locations.end())
		{
			matched = path;
			break ;
		}
		size_t	parentPath = path.find_last_of('/');
		if (parentPath == std::string::npos)
			break ;
		path = path.substr(0, parentPath);
	}
	return (matched);
}

/// @brief Extracts and normalizes the path from a given URI.
///
/// This function removes the trailing slash from the URI path if it exists. 
/// The URI is processed to return the normalized path, which does not end with a slash.
/// @param uri 
/// @return extracted path
std::string _extractPathFromUri(const std::string& uri)
{
	std::string path = uri;
	if (!path.empty() && path[path.size() - 1] == '/')
		path.erase(path.size() - 1);
	return (path);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief This function checks if the request method is in the list of allowed methods.
/// When the Location object is created, it is initialized with a list of allowed methods.
/// if user did not specify the allowed methods at `.conf file`, it is initialized with {"GET", "POST", "DELETE"}.
bool	RequestHandler::_isAllowedMethod(const std::string &target, std::vector<std::string> list) const
{
	if (!list.empty() && std::find(list.begin(), list.end(), target) != list.end())
			return (true);
	return (false);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Processes the HTTP request based on its method.
///
/// This function handles an HTTP request by dispatching it to the appropriate method handler
/// (`GET`, `POST`, or `DELETE`). If the method is not supported, it returns a `501 Not Implemented`
/// response.
///
/// @param request The `HttpRequest` object representing the incoming HTTP request.
/// @param location The `Location` object associated with the request's URI.
/// @return An `HttpResponse` object containing the result of processing the request based on the method.
///         If the method is not supported, returns a `501 Not Implemented` response.
HttpResponse	RequestHandler::_processRequest(const HttpRequest& request,
												const Location& location)
{
	if (request.getMethod() == "GET")
		return (_handleGet(request, location));
	else if (request.getMethod() == "POST")
		return (_handlePost(request, location));
	else if (request.getMethod() == "DELETE")
		return (_handleDelete(request, location));
	else
		return (HttpResponse::notImplemented_501());
}

HttpResponse RequestHandler::_handleGet(const HttpRequest& request,
										const Location& location)
{

	return (_staticFileHandler.handleRequest(request, location));
}

HttpResponse RequestHandler::_handlePost(const HttpRequest& request,
										const Location& location)
{
	(void) request;
	(void) location;
	return (HttpResponse::notImplemented_501());
}

HttpResponse RequestHandler::_handleDelete(const HttpRequest& request,
											const Location& location)
{
	(void) request;
	(void) location;
	return (HttpResponse::notImplemented_501());
}

////////////////////////////////////////////////////////////////////////////////

// TODO: Redirections
// Redirections	https://developer.mozilla.org/en-US/docs/Web/HTTP/Redirections
// request | get -> response -> 301 -> request | get -> response 
/*
<head>
  <meta http-equiv="Refresh" content="0; URL=https://example.com/" />
</head>
*/
