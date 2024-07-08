/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/08 23:35:53 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <map>

class	Config
{
	public:
		Config();
		~Config();

		static void			load(const std::string filename);
		static std::string	get(const std::string key);
		static int			getInt(const std::string key);
		static int			getPort();

	private:
		static std::map<std::string, std::string>	_configMap;
		
		static void			_parseConfigFile(const std::string filename);
};

#endif