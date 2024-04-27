/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/26 23:51:38 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"
#include "ConfigHandler.hpp"


Server			*server;
FileHandler		*config;
ConfigHandler	*config_file;


void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
	server->stop();
	delete server;
	exit(0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Need a .conf file to run the server" <<std::endl;
		return (EXIT_FAILURE);
	}
	config = new FileHandler(argv[1]);
	server = new Server(8080);
	config_file = new ConfigHandler(config->get_content());

	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	delete config;
	delete config_file;

	server->start();
	delete server;
	return (EXIT_SUCCESS);
}
