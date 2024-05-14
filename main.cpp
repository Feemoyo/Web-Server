/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/08 00:03:45 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include "Server.hpp"

Server			*server;

void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
	server->stop();
	delete server;
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	(void)argv;
	
	if (argc != 2)
	{
		std::cerr << "Need a .conf file to run the server" <<std::endl;
		return (EXIT_FAILURE);
	}
	server = new Server(8080);


	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	server->start();
	delete server;
	
	return (EXIT_SUCCESS);
}
