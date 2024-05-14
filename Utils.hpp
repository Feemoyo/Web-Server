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

# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <poll.h>

typedef struct	s_location
{
	std::string					path;
	std::string					default_file;
	std::vector<std::string>	methods;
}	t_location;

typedef struct	s_server
{
	int									port;
	std::string							server_name;
	std::string							root;
	int									max_body_size;
	std::map<std::string, t_location>	locations;
}	t_server;

typedef	enum	e_type
{
	INVALID,
	CONF,
	HTML,
	PNG,
	SVG
}	t_type;

#endif
