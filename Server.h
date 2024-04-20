#pragma once

#ifndef SERVER_H
# define SERVER_H

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>

# include <csignal>

class	Server
{
private:
	int					server_socket;
	int					port;
	static const int	MAX_CLIENTS = 10;

	void				process_request(int client_socket);

public:
			Server(void);
			Server(int n_port);

			~Server(void);

	void	start(void);
	// void	stop(void);

};

#endif