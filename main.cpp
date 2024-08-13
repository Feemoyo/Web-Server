/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/08/13 17:42:19 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Servers.hpp"

Servers		*webserv;
Config		*conf;

void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << "\n";;
	webserv->close_servers();
	delete webserv;
	delete conf;
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Need a .conf file to run the server" << "\n";;
		std::cout << ".conf file exemple:" << "\n";;
		return (EXIT_FAILURE);
	}
	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	conf = new Config(argv[1]);
	if (conf->is_good_to_run() == true)
	{
		webserv = new Servers(conf);
		webserv->run_servers();
	}

	delete conf;
	delete webserv;
	return (EXIT_SUCCESS);
}
