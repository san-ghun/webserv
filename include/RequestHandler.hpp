/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 16:23:00 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/03 17:42:15 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "StaticFileHandler.hpp"

class	RequestHandler
{
	public:
		RequestHandler();
		~RequestHandler();

		HTTPResponse		handleRequest(const HTTPRequest request);

	private:
		StaticFileHandler	_staticFileHandler;

};
#endif