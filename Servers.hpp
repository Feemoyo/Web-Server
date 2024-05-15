#pragma once

#ifndef SERVERSHANDLER_HPP
# define SERVERSHANDLER_HPP

# include "Config.hpp"
# include "Response.hpp"
# include "Request.hpp"

class	Servers
{
private:
	std::vector<t_server>		_servers;
	std::vector<struct pollfd>	_fds;
	Response					_response;
	Request						_request;

			Servers(void);

	void	_init_socket(void);
	bool	_init_bind(void);
	void	_init_listen(void);
	void	_init_fds(void);
	void	_accept_connection(size_t index);
	void	_process_request(size_t index, int &client_fd);

public:
			Servers(Config *&config);
			~Servers(void);

	void	run_servers(void);
	void	close_servers(void);

};

#endif