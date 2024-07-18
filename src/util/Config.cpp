/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/18 18:40:17 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

Config::Config()
{
	// TODO: Set default value at here if necessary
}

Config::~Config()
{
	// TODO: Destruct at here if necessary
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

		// Parse the line
		if (line[line.length() - 1] == '{')
		{
			std::string blockName = line.substr(0, line.size() - 1);
			blockName.erase(blockName.find_last_not_of(" \t") + 1);
			_parseBlock(file, blockName);
		}
		else
		{
			_parseKeyValuePair(line);
		}
	}
}

void	Config::_parseBlock(std::istream& stream, const std::string& blockName)
{
	std::string line;
	while (std::getline(stream, line))
	{
		// Remove comments
		size_t pos = line.find('#');
		if (pos != std::string::npos)
			line = line.substr(0, pos);

		// Trim whitespace
		line.erase(0, line.find_first_not_of(" \t"));
		line.erase(line.find_last_not_of(" \t") + 1);

		if (line[line.length() - 1] == '}')
			return;

		if (line.empty())
			continue;
		
		if (line[line.length() - 1] == '{')
		{
			std::string nestedBlockName = line.substr(0, line.size() - 1);
			nestedBlockName.erase(nestedBlockName.find_last_not_of(" \t") + 1);
			_parseBlock(stream, nestedBlockName);
		}
		else
		{
			if (blockName == "types")
			{
				size_t delimPos = line.find_last_of("\t ");
				std::string value = line.substr(0, delimPos);
				std::string key = line.substr(delimPos + 1);
				key.erase(key.length() - 1);
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);
				_mimeTypeMap[key] = value;
			}
			else if (blockName == "error_pages")
			{
				std::istringstream iss(line);
				int code;
				std::string path;
				iss >> code >> path;
				path.erase(path.length() - 1);
				_errorPageMap[code] = path;
			}
			else if (blockName == "server")
			{
				_parseKeyValuePair(line);
			}
			else if (blockName.rfind("location", 0) == 0)
			{
				std::cout << "Encountered location block: " << line << std::endl;
				// TODO: Parse location block
			}
		}
	}
}

void	Config::_parseKeyValuePair(const std::string line)
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

	_configMap[key] = value;
}

void	Config::_setHostPort()
{
	std::string listen = get("listen");
	size_t delimPos = listen.find_first_of(":");
	// _port = atoi(listen.substr(delimPos + 1).c_str());
	_port = atoi(listen.substr(delimPos + 1).c_str());
	_host = listen.substr(0, delimPos);
}

void	Config::load(const std::string& filename)
{
	_parseConfigFile(filename);
	_setHostPort();
}

void	Config::setLocation(const std::string key)
{
	(void)key;
}

std::map<std::string, std::string>	Config::getConfigMap() const
{
	return (_configMap);
}

std::map<std::string, Location>	Config::getLocationMap() const
{
	return (_locationMap);
}

std::map<std::string, std::string>	Config::getServerSettingMap() const
{
	return (_serverSettingMap);
}

std::map<int, std::string>	Config::getErrorPageMap() const
{
	return (_errorPageMap);
}

std::map<std::string, std::string>	Config::getMimeTypeMap() const
{
	return (_mimeTypeMap);
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

std::string	Config::getServerHost() const
{
	return (_host);
}

int	Config::getPort() const
{
	return (_port);
}
