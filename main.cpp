/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/14 18:18:30 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Servers.hpp"

Servers		*webserv;
Config		*conf;

void	_server_config_model(void)
{
	std::cout << "server\n{\n\tlisten\t\t\t\t\t80" << std::endl;
	std::cout << "\tserver_name\t\t\t\tlocalhost" << std::endl;
	std::cout << "\troot\t\t\t\t\twww\n\tclient_max_body_size\t200" << std::endl;
	std::cout << "\tlocation /\n\t{\n\t\tdefault\t\t\t\tindex.html" << std::endl;
	std::cout << "\t\tallowed_methods\t\tGET\n\t}\n" << std::endl;
	std::cout << "\tlocation /errors/\n\t{\n\t\tdefault\t\t\t\t404.html" << std::endl;
	std::cout << "\t\tallowed_methods\t\tGET\n\t}\n}" << std::endl;
	return ;
}

void	_server_interrupt(int sig)
{
	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
	webserv->close_servers();
	delete webserv;
	delete conf;
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Need a .conf file to run the server" << std::endl;
		std::cout << ".conf file exemple:" << std::endl;
		_server_config_model();
		return (EXIT_FAILURE);
	}
	struct sigaction	interrupt_handler;
	interrupt_handler.sa_handler = _server_interrupt;
	sigemptyset(&interrupt_handler.sa_mask);
	interrupt_handler.sa_flags = 0;
	sigaction(SIGINT, &interrupt_handler, 0);

	conf = new Config(argv[1]);
	webserv = new Servers(conf);

	webserv->run_servers();

	delete conf;
	delete webserv;
	return (EXIT_SUCCESS);
}
