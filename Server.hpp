#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Response.hpp"

class	Server
{
private:
	int					_server_socket;
	int					_port;
	sockaddr_in			_server_addr;
	static const int	_MAX_CLIENTS = 10;
	struct pollfd		_fds[_MAX_CLIENTS + 1];
	int					_active_clients;

	int					_NUM_REQUEST;
	Response			_response;

						Server(void);

	int					_accept_request(void);
	void				_process_request(int client_socket);

	void				_init_socket(void);
	void				_init_bind(void);
	void				_init_listen(void);
	void				_init_fds(void);

public:
						Server(int port);
						~Server(void);

	void				start(void);
	void				stop(void);

};

#endif
