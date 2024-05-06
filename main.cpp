/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/06 10:33:53 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ToolKit.hpp"

Server			*server;
ToolKit			*tools;

void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
	server->stop();
	delete server;

	delete tools;

	exit(EXIT_SUCCESS);
}

#include "ToolKit.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Need a .conf file to run the server" <<std::endl;
		return (EXIT_FAILURE);
	}
	server = new Server(8080);
	tools = new ToolKit();

	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	tools->set_file("./", argv[1]);

	std::cout << tools->get_file_name() << std::endl;
	std::cout << tools->get_file_path() << std::endl;

	server->start();
	delete server;
	delete tools;
	
	return (EXIT_SUCCESS);
}
