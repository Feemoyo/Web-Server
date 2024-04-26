/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/26 12:59:13 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileHandler.hpp"

Server			*server;
FileHandler		*config;

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

	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	delete config;

	server->start();
	delete server;
	return (EXIT_SUCCESS);
}
