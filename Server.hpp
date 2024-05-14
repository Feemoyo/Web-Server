#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"
# include "Response.hpp"
# include "Request.hpp"

class	Server
{
private:
	t_server			_server;

						Server(void);
	// void				_process_request(int client_socket);

	void				_init_socket(void);
	void				_init_bind(void);
	void				_init_listen(void);

public:
						Server(t_server &server);
						~Server(void);

	void				accept_connection(struct pollfd &fds);

	int					&get_server_port(void);
	int					&get_server_socket(void);


};

#endif
