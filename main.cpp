/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmoreira <fmoreira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 19:27:55 by rferrero          #+#    #+#             */
/*   Updated: 2024/05/02 19:58:49 by fmoreira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Server.hpp"
// #include "FileHandler.hpp"
// #include "Parser.hpp"
// #include "Response.hpp"

// Server			*server;
// FileHandler		*config;
// Parser			*config_file;


// void	_server_interrupt(int sig)
// {
// 	std::cout << "\nServer Interrupted\nSignal: " << (sig + 128) << std::endl;
// 	server->stop();
// 	delete server;
// 	exit(EXIT_SUCCESS);
// }

#include "ToolKit.hpp"

int main()
{
	ToolKit	*t = new ToolKit();

	t->set_file("/dir/www/index.html");

	std::string tmp = t->get_file_name();
	std::string tmp2 = t->get_file_path(true);
	
	std::cout << "filename: " << tmp << std::endl;
	std::cout << "filepath: " << tmp2 << std::endl;

	// if (argc != 2)
	// {
	// 	std::cerr << "Need a .conf file to run the server" <<std::endl;
	// 	return (EXIT_FAILURE);
	// }
	// config = new FileHandler(argv[1]);
	// server = new Server(8080);
	// config_file = new Parser(config->get_content());

	// struct sigaction	interrupt_handler;
	// interrupt_handler.sa_handler = _server_interrupt;
	// sigemptyset(&interrupt_handler.sa_mask);
	// interrupt_handler.sa_flags = 0;
	// sigaction(SIGINT, &interrupt_handler, 0);

	// delete config;
	// delete config_file;

	// server->start();
	// delete server;

	// Response response(0);
	// response.send_response();
	
	// return (EXIT_SUCCESS);
}
