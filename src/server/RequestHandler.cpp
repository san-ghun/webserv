/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minakim <minakim@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/11 19:53:06 by minakim          ###   ########.fr       */
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
