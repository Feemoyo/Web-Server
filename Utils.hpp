#pragma once

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <ostream>
# include <string>
# include <vector>
# include <map>
# include <list>
# include <algorithm>

# include <cstring>
# include <cstdlib>
# include <csignal>
# include <cctype>
# include <ctime>
# include <cstdio>

# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <netdb.h>
# include <fcntl.h>

# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>
# include <dirent.h>

# define MAX_CLIENTS 10

typedef struct	s_redirect
{
	std::string		path;
	std::string		redir;
}	t_redirect;

typedef struct	s_location
{
	std::string					path;
	std::string					default_file;
	bool						directory;
	std::vector<std::string>	methods;
}	t_location;

typedef struct	s_server
{
	int									port;
	std::string							server_name;
	std::string							root;
	bool								directory;
	int									max_body_size;
	std::map<std::string, t_location>	locations;
	std::vector<t_redirect>				redirects;

	int									server_socket;
	sockaddr_in							server_addr;
}	t_server;

typedef struct	s_response
{
	int				client;
	t_server		server;
	std::string		method;
	std::string		path;
	std::string		name;
	std::string		payload;

	std::string		header;
	std::string		body;

}	t_response;

typedef struct	s_cgi
{
	std::string	method;
	std::string	cgi;
	std::string	script_file;
	std::string	data;
}	t_cgi;


std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs);
std::ostream	&operator<<(std::ostream &lhs, const t_server &rhs);

#endif
