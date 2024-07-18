/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/17 15:37:51 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"
# include "Server.hpp"

class	Server;

class	Location
{
	public:
		Location();
		Location(Server* server, std::string path);
		~Location();

		// Getters
		Server*								getServer() const;
		std::string							getPath() const;
		std::string							getRootPath() const;
		std::string							getUploadPath() const;
		std::string							getIndex() const;
		std::vector<std::string>			getAllowedMethods() const;
		std::string							getRedirectPath() const;
		bool								isRedirect() const;
		int									getRedirectCode() const;
		std::map<std::string, std::string>	getCgi() const;
		// Setters
		void								setPath(std::string path);
		void								setRootPath(std::string rootPath);
		void								setUploadPath(std::string uploadPath);
		void								setIndex(std::string index);
		void								setAllowedMethods(std::vector<std::string> allowedMethods);
		void								setRedirect(std::string redirectPath);
		void								setCgi(std::string cgi);

	private:
		Server*								_server;
		std::string							_path;
		std::string							_rootPath;
		std::string							_uploadPath;
		std::string							_index;
		std::vector<std::string>			_allowedMethods;
		std::string							_redirectPath;
		bool								_isRedirect;
		int									_redirectCode;
		std::map<std::string, std::string>	_cgi;
};

#endif