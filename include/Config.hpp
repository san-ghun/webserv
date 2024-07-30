/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/30 17:52:14 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include "Location.hpp"

class Location;

struct LocationConfig 
{
	std::string path;
    std::string root;
    std::vector<std::string> allowed_methods;
	std::string upload_dir;
    bool lsdir;
    std::string cgi_ext;
    std::string redirection;
    std::string default_file;
};

struct ServerConfig 
{
    std::string server_name;
    std::string listen;
	std::string host;
	int port;
    size_t max_body_size;
    std::string root;
    std::string default_file;
    std::string upload_dir;
	std::string cgi_dir;
    std::map<int, std::string> error_pages;
    std::map<std::string, LocationConfig*> map_locations;
	std::map<std::string, Location*> map_locationObjs;
};

LocationConfig*	new_locationConfig();
ServerConfig*	new_serverConfig();

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

		// TODO: Implement
		ServerConfig*	getServerByName(const std::string serverName) const;
		ServerConfig*	getServerByListen(const std::string serverListen) const;
		ServerConfig*	getServerByHost(const std::string serverHost) const;
		ServerConfig*	getServerByPort(const int serverPort) const;

		// to server class ...?
		// std::string		getServerHost() const;
		// int				getPort() const;
		// std::string		getErrorPage(const int code) const;
		// Location*		getLocation(const std::string key) const;

		std::vector<ServerConfig*>			getServers() const;
		std::map<std::string, std::string>	getMimeTypeMap() const;
		std::map<std::string, std::string>	getConfigMap() const;

		// to server class ...?
		// std::map<int, std::string>			getErrorPageMap() const;
		// std::map<std::string, std::string>	getServerSettingMap() const;
		// std::map<std::string, Location*>	getLocationMap() const;

		// TODO: Implement
		void			setServer(ServerConfig* server, std::string line);
		void			setLocation(Location* location, std::string line);

	private:
		// Singleton: Private constructor and destructor
		Config();
		~Config();

		void			_parseConfigFile(const std::string& filename);
		// void			_setHostPort();

		std::vector<ServerConfig*>			_servers;
		std::map<std::string, std::string>	_mimeTypeMap;
		std::map<std::string, std::string>	_configMap;

		// to server class ...?
		// std::map<int, std::string>			_errorPageMap;
		// std::map<std::string, std::string>	_serverSettingMap;
		// std::map<std::string, Location*>	_locationMap;

		// int				_port;
		// std::string		_host;
};

#endif
