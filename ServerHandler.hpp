#pragma once

#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

# include "Utils.hpp"
# include "Server.hpp"

class	ServerHandler
{
private:
	std::vector<Server>	_servers;

			ServerHandler(void);

public:
			ServerHandler(std::vector<Server> &servers);
			~ServerHandler(void);

	void	run_servers(void);

};

#endif