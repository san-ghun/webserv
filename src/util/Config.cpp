/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/12 16:47:45 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"

Config::Config()
{
	// TODO: Set default value at here if necessary
}

Config::~Config()
{
	// TODO: Destruct at here if necessary
}

void	Config::load(const std::string filename)
{
	_parseConfigFile(filename);
}

std::string	Config::get(const std::string key)
{}

int	Config::getInt(const std::string key)
{}

int	Config::getPort()
{}

Location	Config::getLocation(const std::string key)
{}

std::map<std::string, std::string>	Config::getConfigMap()
 {
	return (_configMap);
}

void	Config::_parseConfigFile(const std::string filename)
{
	// TODO: Parse config file
	
}
