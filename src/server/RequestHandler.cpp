/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 14:30:32 by minakim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "RequestHandler.hpp"
#include "StaticFileHandler.hpp"
#include "HttpResponse.hpp"

RequestHandler::RequestHandler()
{}

RequestHandler::~RequestHandler()
{}

/// @brief 
/// @param request HttpRequest object
/// @return 1. if request is `GET`, 2. if request is `POST`,
///			3. if request is `DELETE`, 4. if request is `not implemented`.
/// @note `501 Not Implemented`: temp response, the server does not support
/// 		the functionality required to fulfill the request.
HttpResponse         RequestHandler::handleRequest(const HttpRequest request)
{
	if (request.getMethod() == "GET")
		return (_staticFileHandler.handleRequest(request));
	else if (request.getMethod() == "POST")
		return (HttpResponse::notImplemented_501());
	else if (request.getMethod() == "DELETE")
		return (HttpResponse::notImplemented_501());
	else
		return (HttpResponse::notImplemented_501());
}
