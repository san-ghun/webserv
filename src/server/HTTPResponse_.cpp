/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse_.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 10:55:55 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
{}

HTTPResponse::~HTTPResponse()
{}

void								HTTPResponse::setStatusCode(int code, const std::string statusMessage)
{}

void								HTTPResponse::setHeader(const std::string key, const std::string value)
{}

void								HTTPResponse::setBody(const std::string bodyContent)
{}

std::string							HTTPResponse::toString() const
{}

HTTPResponse					HTTPResponse::fromFile(const std::string filePath)
{}

HTTPResponse					HTTPResponse::badRequest_400()
{}

HTTPResponse					HTTPResponse::forbidden_403()
{}

HTTPResponse					HTTPResponse::notFound_404()
{}

HTTPResponse					HTTPResponse::methodNotAllowed_405()
{}

HTTPResponse					HTTPResponse::requestTimeout_408()
{}

HTTPResponse					HTTPResponse::requestEntityTooLarge_413()
{}

HTTPResponse					HTTPResponse::imaTeapot_418()
{}

HTTPResponse					HTTPResponse::internalServerError_500()
{}

/**
int									_statusCode
std::string							_statusMessage
std::map<std::string, std::string>	_headers
std::string							_body
 */

std::string							HTTPResponse::_getStatusLine() const
{}

std::string							HTTPResponse::_getHeadersString() const
{}
