/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/17 15:36:40 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include "Location.hpp"

class Location;

class	Config
{
	public:
		// Singleton: Static method to get the single instance of the class
		static Config&	getInstance();

		// Deleted copy constructor and copy assignment operator to prevent copying
		// Config(const Config&) = delete;
		// void operator=(const Config&) = delete;

		void			load(const std::string& filename);
		
		std::string		get(const std::string key) const;
		int				getInt(const std::string key) const;
		bool			getBool(const std::string key) const;
		std::string		getServerHost() const;
		int				getPort() const;

		std::map<std::string, std::string>	getMimeTypeMap() const;
		std::map<int, std::string>			getErrorPageMap() const;
		std::map<std::string, std::string>	getServerSettingMap() const;
		std::map<std::string, Location>		getLocationMap() const;
		std::map<std::string, std::string>	getConfigMap() const;

		void			setLocation(const std::string key);

	private:
		// Singleton: Private constructor and destructor
		Config();
		~Config();

		void			_parseKeyValuePair(const std::string line);
		void			_parseBlock(std::istream& stream, const std::string& blockName);
		void			_parseConfigFile(const std::string& filename);
		void			_setHostPort();

		std::map<std::string, std::string>	_mimeTypeMap;
		std::map<int, std::string>			_errorPageMap;
		std::map<std::string, std::string>	_serverSettingMap;
		std::map<std::string, Location>		_locationMap;
		std::map<std::string, std::string>	_configMap;

		int				_port;
		std::string		_host;
};

#endif
