#pragma once

#ifndef SERVERS_HPP
# define SERVERS_HPP

# include "Config.hpp"
# include "Response.hpp"
# include "Client.hpp"

class	Servers
{
private:
	std::vector<t_server>		_servers;
	std::vector<struct pollfd>	_fds;
	Client						_client;

			Servers(void);

	void	_init_socket(void);
	bool	_init_bind(void);
	void	_init_listen(void);
	void	_init_fds(void);
	void	_accept_connection(size_t index);
  	void	_process_client(size_t index, int &client_fd);
	void	_process_response(size_t index, int &client_fd, std::string method, std::string payload);

public:
			Servers(Config *&config);
			~Servers(void);

	void	run_servers(void);
	void	close_servers(void);

};

#endif