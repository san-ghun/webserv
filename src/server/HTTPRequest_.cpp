/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest_.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 10:52:48 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest()
{}

HTTPRequest::~HTTPRequest()
{}

bool								HTTPRequest::parse(const std::string requestData)
{}

std::string							HTTPRequest::getMethod() const
{}

std::string							HTTPRequest::getPath() const
{}

std::string							HTTPRequest::getVersion() const
{}

std::map<std::string, std::string>	HTTPRequest::getHeaders() const
{}

std::string							HTTPRequest::getBody() const
{}

bool								HTTPRequest::isConnectionClose() const
{}


/**
std::string							_method
std::string							_path
std::string							_version
std::map<std::string, std::string>	_headers
std::string							_body
 */

void								HTTPRequest::_parseRequestLine(const std::string requestLine)
{}

void								HTTPRequest::_parseHeaders(const std::string headerLines)
{}

void								HTTPRequest::_parseBody(const std::string bodylines)
{}

