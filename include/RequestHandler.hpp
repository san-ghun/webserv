/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/23 22:24:17 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

class HttpResponse;
class HttpRequest;
class Location;

# include <algorithm>
# include "StaticFileHandler.hpp"
# include "webserv.hpp"

std::string _extractPathFromUri(const std::string& uri);
std::string _getMatchedLocation(std::string path,
								const std::map<std::string,
								Location*>& locations);
class	RequestHandler
{
	public:
		RequestHandler();
		~RequestHandler();

		HttpResponse		handleRequest(const HttpRequest& request);

	private:
		StaticFileHandler	_staticFileHandler;

		Location*			_findLocation(const std::string& uri);
		HttpResponse		_processRequest(const HttpRequest& request, const Location& location);
	
		bool				_isAllowedMethod(const std::string &target, std::vector<std::string> list) const;
		HttpResponse		_handleGet(const HttpRequest& request, const Location& location);
    	HttpResponse		_handlePost(const HttpRequest& request, const Location& location);
    	HttpResponse		_handleDelete(const HttpRequest& request, const Location& location);
};
#endif