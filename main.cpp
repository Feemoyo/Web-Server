/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/14 01:50:49 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ToolKit.hpp"
#include "Config.hpp"
#include "ServerHandler.hpp"


void	_server_config_model(void)
{
	std::cout << "server\n{\n\tlisten\t\t\t\t\t80\n\tserver_name\t\t\t\tlocalhost\n\troot\t\t\t\t\twww\n\tclient_max_body_size\t200\n\tlocation /\n\t{\n\t\tdefault\t\t\t\tindex.html\n\t\tallowed_methods\t\tGET\n\t}\n\tlocation /errors/\n\t{\n\t\tdefault\t\t\t\t404.html\n\t\tallowed_methods\t\tGET\n\t}\n}" << std::endl;
	return ;
}

// void	_server_interrupt(int sig)
// {
// 	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
// 	for (size_t i = 0; i < servers.size(); i++)
// 		servers[i].stop();
// 	exit(EXIT_SUCCESS);
// }

// void	_server_signal(void)
// {
// 	struct sigaction	interrupt_handler;
// 	interrupt_handler.sa_handler = _server_interrupt;
// 	sigemptyset(&interrupt_handler.sa_mask);
// 	interrupt_handler.sa_flags = 0;
// 	sigaction(SIGINT, &interrupt_handler, 0);

// }

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Need a .conf file to run the server" << std::endl;
		std::cout << ".conf file exemple:" << std::endl;
		_server_config_model();
		return (EXIT_FAILURE);
	}

	ToolKit	tools;
	tools.set_file("./", argv[1]);
	Config	conf(tools.get_content());

	std::vector<Server>	servers;
	for (size_t i = 0; i < conf.get_total_servers(); i++)
	{
		Server	servidor(conf.get_server(i));
		servers.push_back(servidor);
	}

	ServerHandler	servidores(servers);
	servidores.run_servers();

	return (EXIT_SUCCESS);
}
