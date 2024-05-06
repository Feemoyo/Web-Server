#pragma once

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <vector>
# include <map>
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
	std::vector<std::string>	methods;
	std::string					index_file;
	bool						autoindex;
}	t_location;

typedef struct	s_server
{
	std::string							port;
	std::string							server_name;
	std::string							root;
	std::map<std::string, std::string>	errors;
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