/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/04/23 18:33:12 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "FileReader.hpp"

Server	*server;

void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
	server->stop();
	delete server;
	exit(0);
}

int main(void)
{
	server = new Server(8080);

	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	FileReader	*fr = new FileReader("index.png");

	std::cout << fr->get_type() << std :: endl;

	server->start();
	delete server;
	return (0);
}
