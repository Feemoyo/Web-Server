/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/06/22 18:59:11 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Servers.hpp"

Servers		*webserv;
Config		*conf;

void	_server_config_model(void)
{
	std::cout << "server\n{\n\tlisten\t\t\t\t\t80" << "\n";;
	std::cout << "\tserver_name\t\t\t\tlocalhost" << "\n";;
	std::cout << "\troot\t\t\t\t\twww" << "\n";;
	std::cout << "\tdirectory\t\t\t\ton\n\tclient_max_body_size\t\t\t200\n" << "\n";;
	std::cout << "\tlocation /\n\t{\n\t\tdefault\t\t\t\tindex.html" << "\n";;
	std::cout << "\t\tdirectory\t\t\ton" << "\n";;
	std::cout << "\t\tallowed_methods\t\t\tGET\n\t}\n" << "\n";;
	std::cout << "\tlocation /errors/\n\t{\n\t\tdefault\t\t\t\t404.html" << "\n";;
	std::cout << "\t\tdirectory\t\t\toff" << "\n";;
	std::cout << "\t\tallowed_methods\t\t\tGET\n\t}\n}" << "\n";;
	
}

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
		_server_config_model();
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
