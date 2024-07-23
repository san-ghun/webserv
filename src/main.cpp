/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sanghupa <sanghupa@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 21:05:14 by sanghupa          #+#    #+#             */
/*   Updated: 2024/07/22 23:45:33 by sanghupa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
// #include "util/Logger.hpp"
#include "Server.hpp"

volatile bool	g_sigint = false;

/// @brief Handles the SIGINT signal by printing a message and exiting the program.
/// @param sig the signal number
/// @return void
/// @throws None
static void	ft_sigint_handler(int sig)
{
	(void)sig;
	std::cout << "\rserver shutting down..." << std::endl;
	g_sigint = true;
	// exit(0);
}

/// @brief The main function of the program.
/// It reads the port number from the command line arguments and runs the server.
/// @param argc The number of command line arguments.
/// @param argv An array of character pointers to command line arguments.
/// @return The exit status of the program.
/// 
/// The program expects one command line argument: the path to the configuration file.
/// If the argument is missing, the program prints the usage message and exits with status 1.
/// Otherwise, it runs the server on the specified port.
int main(int argc, char *argv[])
{
	// Check if the correct number of arguments is provided
	if (argc != 2)
	{
		// Print the usage message and exit with status 1
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	// Ignore SIGPIPE signals
	signal(SIGPIPE, SIG_IGN);
	// Set the signal handler for SIGINT signal
	signal(SIGINT, ft_sigint_handler);

	// try-catch block for error handling
	// Create a Config object with the provided configuration file
	Config& config = Config::getInstance();
	config.load(argv[1]);
	// Create a Logger object with the Config object
	// Logger logger(config);
	// Create a Server object with the Config object
	Server server(config);
	// Server server(8080);
	// Start the server
	server.start();

	// Exit with status 0
	return (0);
}
