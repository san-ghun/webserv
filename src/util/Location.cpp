/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/22 23:01:58 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"

Location::Location()
{
	_server = NULL;
	_path = "";
	_rootPath = "";
	_isListdir = false;
	_uploadPath = "";
	_index = "";
	_allowedMethods = std::vector<std::string>();
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	_isRedirect = false;
	_redirectPath = "";
	_redirectCode = "";
	_cgi = std::map<std::string, std::string>();
}

Location::Location(std::string path)
	: _path(path)
{
	_rootPath = "/www/static";
	_isListdir = false;
	_uploadPath = "/www/data/uploads";
	_index = "index.html";
	_allowedMethods = std::vector<std::string>();
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	_isRedirect = false;
	_redirectPath = "";
	_redirectCode = "";
	_cgi = std::map<std::string, std::string>();
}

Location::Location(Server* server, std::string path)
	: _server(server), _path(path)
{
	_rootPath = "/www/static";
	_isListdir = false;
	_uploadPath = "/www/data/uploads";
	_index = "index.html";
	_allowedMethods = std::vector<std::string>();
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	_isRedirect = false;
	_redirectPath = "";
	_redirectCode = "";
	_cgi = std::map<std::string, std::string>();
}

Location::~Location()
{}

Server*	Location::getServer() const
{
	return (_server);
}

std::string	Location::getPath() const
{
	return (_path);
}

std::string	Location::getRootPath() const
{
	return (_rootPath);
}

bool	Location::isListdir() const
{
	return (_isListdir);
}

std::string	Location::getUploadPath() const
{
	return (_uploadPath);
}

std::string	Location::getIndex() const
{
	return (_index);
}

std::vector<std::string>	Location::getAllowedMethods() const
{
	return (_allowedMethods);
}

std::string	Location::getRedirectPath() const
{
	return (_redirectPath);
}

bool	Location::isRedirect() const
{
	return (_isRedirect);
}

std::string	Location::getRedirectCode() const
{
	return (_redirectCode);
}

std::map<std::string, std::string>	Location::getCgi() const
{
	return (_cgi);
}

void	Location::setServer(Server* server)
{
	_server = server;
}

void	Location::setPath(std::string path)
{
	_path = path;
}

void	Location::setRootPath(std::string rootPath)
{
	_rootPath = rootPath;
}

void	Location::setIsListdir(bool isListdir)
{
	_isListdir = isListdir;
}

void	Location::setUploadPath(std::string uploadPath)
{
	_uploadPath = uploadPath;
}

void	Location::setIndex(std::string index)
{
	_index = index;
}

void	Location::setAllowedMethods(std::vector<std::string> allowedMethods)
{
	_allowedMethods.clear();
	_allowedMethods = allowedMethods;
}

void	Location::setRedirect(std::string redirectPath)
{
	_redirectPath = redirectPath;
	_isRedirect = true;
	_redirectCode = "307";
}

void	Location::setCgi(std::string cgi)
{
	_cgi["cgi"] = cgi;
}
