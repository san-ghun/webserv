/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/08/02 21:59:24 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

LocationConfig*	new_locationConfig()
{
	LocationConfig*	_this = new LocationConfig();

	_this->path = "";
	_this->root = "";
	_this->allowed_methods = std::vector<std::string>();
	_this->upload_dir = "";
	_this->lsdir = false;
	_this->cgi_ext = ".py";
	_this->redirection = "";
	_this->default_file = "index.html";

	return _this;
}

ServerConfig*	new_serverConfig()
{
	ServerConfig*	_this = new ServerConfig();

	_this->server_name = "";
	_this->listen = "localhost:80";
	_this->host = "localhost";
	_this->port = 80;
	_this->max_body_size = 1024;
	_this->root = "/www";
	_this->default_file = "index.html";
	_this->upload_dir = "/www/data/uploads";
	_this->cgi_dir = "/www/cgi-bin";
	_this->error_pages = std::map<int, std::string>();
	_this->map_locations = std::map<std::string, LocationConfig*>();
	_this->map_locationObjs = std::map<std::string, Location*>();
	
	return _this;
}

Config::Config()
{
	// TODO: Set default value at here if necessary
}

Config::~Config()
{
	// TODO: Destruct at here if necessary
	for (size_t i = 0; i < _servers.size(); i++)
	{
		ServerConfig* its = _servers[i];
		std::map<std::string, LocationConfig*> lMap = its->map_locations;
		for (std::map<std::string, LocationConfig*>::iterator itl = lMap.begin(); itl != lMap.end(); itl++)
		{
			std::cout << "Delete LocationConfig: " << itl->first << std::endl;
			delete itl->second;
		}
		std::map<std::string, Location*> lobjMap = its->map_locationObjs;
		for (std::map<std::string, Location*>::iterator itl = lobjMap.begin(); itl != lobjMap.end(); itl++)
		{
			std::cout << "Delete Location: " << itl->first << std::endl;
			delete itl->second;
		}
		std::cout << "Delete ServerConfig: " << its->listen << std::endl;
		delete its;
	}

}

/// Returns the single instance of the Config class.
/// @return Config& The single instance of the Config class
/// @throws None
Config&	Config::getInstance()
{
	static Config instance;
	return (instance);
}

void	Config::_parseConfigFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file)
	{
		throw std::runtime_error("Unable to open config file: " + filename);
	}

	std::string line;
	ServerConfig* currentServer = new_serverConfig();
	LocationConfig* currentLocation = new_locationConfig();
	bool inTypesBlock = false;
	bool inServerBlock = false;
	bool inLocationBlock = false;
	bool inErrorPagesBlock = false;

	while (std::getline(file, line))
	{
		// Remove comments
		size_t pos = line.find('#');
		if (pos != std::string::npos)
			line = line.substr(0, pos);

		// Trim whitespace
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		// Skip empty lines
		if (line.empty())
			continue;

		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "types")
		{
			inTypesBlock = true;
		}
		else if (key == "server")
		{
			inServerBlock = true;
			if (currentServer->server_name.empty() == false)
			{
				_servers.push_back(currentServer);
				currentServer = new_serverConfig();
			}
		}
		else if (key == "location")
		{
			inLocationBlock = true;
			if (currentLocation->path.empty() == false)
			{
				currentServer->map_locations[currentLocation->path] = currentLocation;
				currentLocation = new_locationConfig();
			}
			std::string locationPath;
			iss >> locationPath;
			currentLocation->path = locationPath;
		}
		else if (key == "error_pages")
		{
			inErrorPagesBlock = true;
		}
		else if (key == "}")
		{
			if (inErrorPagesBlock)
			{
				inErrorPagesBlock = false;
			}
			else if (inLocationBlock)
			{
				inLocationBlock = false;
				if (currentLocation->allowed_methods.size() == 0)
				{
					currentLocation->allowed_methods.push_back("GET");
					currentLocation->allowed_methods.push_back("POST");
					currentLocation->allowed_methods.push_back("DELETE");
				}
				currentServer->map_locations[currentLocation->path] = currentLocation;
				currentServer->map_locationObjs[currentLocation->path] = new Location(currentLocation);
				currentServer->map_locationObjs[currentLocation->path]->setServer(currentServer);
				currentLocation = new_locationConfig();
			}
			else if (inServerBlock)
			{
				inServerBlock = false;
				_servers.push_back(currentServer);
				currentServer = new_serverConfig();
			}
			else if (inTypesBlock)
			{
				inTypesBlock = false;
			}
		}
		else if (inTypesBlock)
		{
			size_t delimPos = line.find_last_of("\t ");
			std::string v = line.substr(0, delimPos);
			std::string k = line.substr(delimPos + 1);
			k.erase(k.length() - 1);
			v.erase(0, v.find_first_not_of(" \t"));
			v.erase(v.find_last_not_of(" \t") + 1);
			_mimeTypeMap[k] = v;
		}
		else if (inErrorPagesBlock)
		{
			int code;
			code = atoi(key.c_str());
			std::string path;
			iss >> path;
			path.erase(path.length() - 1);
			currentServer->error_pages[code] = path;
		}
		else if (inLocationBlock)
		{
			std::string val = "";
			if (key == "root")
			{
				iss >> val;
				val.erase(val.length() - 1);
				currentLocation->root = val;
			}
			else if (key == "allowed_methods")
			{
				std::string delim = " ";
				size_t pos = 0;
				while ((pos = line.find(delim)) != std::string::npos)
				{
					val = line.substr(0, pos);
					val.erase(0, val.find_first_not_of(" \t"));
					val.erase(val.find_last_not_of(" \t") + 1);
					currentLocation->allowed_methods.push_back(val);
					line.erase(0, pos + delim.length());
				}
				val = line;
				val.erase(val.length() - 1);
				currentLocation->allowed_methods.push_back(val);
			}
			else if (key == "lsdir")
			{
				currentLocation->lsdir = true;
			}
			else if (key == "cgi_ext")
			{
				iss >> val;
				val.erase(val.length() - 1);
				currentLocation->cgi_ext = val;
			}
			else if (key == "redirection")
			{
				iss >> val;
				val.erase(val.length() - 1);
				currentLocation->redirection = val;
			}
			else if (key == "default_file")
			{
				iss >> val;
				val.erase(val.length() - 1);
				currentLocation->default_file = val;
			}
		}
		else if (inServerBlock)
		{
			std::string value = "";
			if (key == "server_name")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->server_name = value;
			}
			else if (key == "listen")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->listen = value;
				std::string listen = currentServer->listen;
				size_t delimPos = listen.find_first_of(":");
				currentServer->port = atoi(listen.substr(delimPos + 1).c_str());
				currentServer->host = listen.substr(0, delimPos);
			}
			else if (key == "max_body_size")
			{
				iss >> currentServer->max_body_size;
			}
			else if (key == "root")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->root = value;
			}
			else if (key == "default_file")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->default_file = value;
			}
			else if (key == "upload_dir")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->upload_dir = value;
			}
			else if (key == "cgi_dir")
			{
				iss >> value;
				value.erase(value.length() - 1);
				currentServer->cgi_dir = value;
			}
		}
	}

	if (currentServer->server_name.empty() == false)
		_servers.push_back(currentServer);

	delete currentServer;
	delete currentLocation;
}

void	Config::load(const std::string& filename)
{
	_parseConfigFile(filename);
}

void	Config::setLocation(Location* location, std::string line)
{
	size_t delimPos = line.find_first_of("\t ");
	std::string key = line.substr(0, delimPos);
	std::string value = line.substr(delimPos + 1);

	// Trim whitespace from key and value
	key.erase(0, key.find_first_not_of(" \t"));
	key.erase(key.find_last_not_of(" \t") + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	value.erase(value.find_last_not_of(" \t") + 1);
	value.erase(value.length() - 1);

	if (key == "root")
		location->setRootPath(this->get("root") + value);
	else if (key == "lsdir")
		location->setIsListdir(true);
	else if (key == "upload_dir")
		location->setUploadPath(this->get("upload_dir") + value);
	else if (key == "default_file")
		location->setIndex(value);
	else if (key == "allowed_methods")
	{
		std::vector<std::string> methods;
		std::string delimiter = " ";
		size_t pos = 0;
		std::string token;
		while ((pos = value.find(delimiter)) != std::string::npos)
		{
			token = value.substr(0, pos);
			token.erase(0, token.find_first_not_of(" \t"));
			token.erase(token.find_last_not_of(" \t") + 1);
			methods.push_back(token);
			value.erase(0, pos + delimiter.length());
		}
		methods.push_back(value);
		location->setAllowedMethods(methods);
	}
	else if (key == "redirection")
		location->setRedirect(value);
	else if (key == "cgi")
		location->setCgi(value);
}

std::map<std::string, std::string>	Config::getConfigMap() const
{
	return (_configMap);
}

// std::map<std::string, Location*>	Config::getLocationMap() const
// {
// 	return (_locationMap);
// }

// std::map<std::string, std::string>	Config::getServerSettingMap() const
// {
// 	return (_serverSettingMap);
// }

// std::map<int, std::string>	Config::getErrorPageMap() const
// {
// 	return (_errorPageMap);
// }

std::map<std::string, std::string>	Config::getMimeTypeMap() const
{
	return (_mimeTypeMap);
}

std::vector<ServerConfig*>	Config::getServers() const
{
	return (_servers);
}

ServerConfig*	Config::getServerByListen(const std::string serverListen) const
{
	for (std::vector<ServerConfig*>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if ((*it)->listen == serverListen)
			return (*it);
	}
	return (NULL);
}

std::string	Config::get(const std::string key) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = _configMap.find(key);
	if (it == _configMap.end())
		return ("");
	return (it->second);
}

bool	Config::getBool(const std::string key) const
{
	std::string value = get(key);
	if (value == "true")
		return (true);
	if (value == "false")
		return (false);
	return (false);
}

int	Config::getInt(const std::string key) const
{
	std::string value = get(key);
	if (value.empty())
		return (0);
	return (atoi(value.c_str()));
}

// std::string	Config::getServerHost() const
// {
// 	return (_host);
// }

// int	Config::getPort() const
// {
// 	return (_port);
// }

// std::string	Config::getErrorPage(const int code) const
// {
// 	std::map<int, std::string>::const_iterator it;
// 	it = _errorPageMap.find(code);
// 	if (it == _errorPageMap.end())
// 		return ("");
// 	return (it->second);
// }

// Location*	Config::getLocation(const std::string key) const
// {
// 	std::map<std::string, Location*>::const_iterator it;
// 	it = _locationMap.find(key);
// 	if (it == _locationMap.end())
// 		return (NULL);
// 	return (it->second);
// }
