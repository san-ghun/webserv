/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 21:04:39 by sanghupa          #+#    #+#             */
/*   Updated: 2024/10/30 23:05:48 by sanghupa         ###   ########.fr       */
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

// Macros
#define MAX_EVENTS 100   // TEST PURPOSE ONLY
#define BUFSIZE 1024   // TEST PURPOSE ONLY
#define PORT 8080   // TEST PURPOSE ONLY

// Functions

extern volatile bool g_sigint;

#define RESET "\033[0m"

// Regular Colors
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"

#endif