/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/08 20:47:32 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

class HttpResponse;
class HttpRequest;
class ServerConfig;
class Location;
class Context;

# include <algorithm>
# include "StaticFileHandler.hpp"
# include "webserv.hpp"

class	RequestHandler
{
	public:
		RequestHandler();
		~RequestHandler();

		// HttpResponse		handleRequest(const HttpRequest& request);
		HttpResponse		handleRequest(const Context& context);

	private:
		StaticFileHandler	_staticFileHandler;

		HttpResponse		_processRequest(const Context& context);
	
		bool				_isAllowedMethod(const Context& context) const;
		HttpResponse		_handleGet(const Context& context);
    	HttpResponse		_handlePost(const Context& context);
    	HttpResponse		_handleDelete(const Context& context);
};
#endif