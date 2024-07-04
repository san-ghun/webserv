/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 21:04:39 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/04 15:59:03 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// C headers
/// TODO: Check if these libs are needed or valid to use
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <poll.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/wait.h>
// # include <sys/stat.h>
// # include <sys/types.h>
// # include <sys/socket.h>
// # include <netinet/in.h>

// C++ headers
/// TODO: Check if these libs are needed or valid to use
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <set>
# include <string>
// # include <algorithm>

// Custom headers
// # include "Config.hpp"
// # include "Location.hpp"
// # include "Logger.hpp"
// # include "Server.hpp"
// # include "RequestHandler.hpp"
// # include "StaticFileHandler.hpp"
// # include "HttpRequest.hpp"
// # include "HttpResponse.hpp"
// # include "Socket.hpp"
// # include "Poller.hpp"

// Macros
#define MAX_EVENTS 100   // TEST PURPOSE ONLY
#define BUFSIZE 1024   // TEST PURPOSE ONLY
#define PORT 8080   // TEST PURPOSE ONLY

// Functions

#endif