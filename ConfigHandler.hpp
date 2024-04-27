#pragma once

#ifndef CONFIGHANDLER_HPP
# define CONFIGHANDLER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

typedef struct	s_location
{
	std::string					path;
	std::vector<std::string>	methods;
	std::string					index_file;
	bool						autoindex;
}	t_location;

typedef struct	s_server
{
	std::string							_port;
	std::string							_server_name;
	std::string							_root;
	std::map<std::string, std::string>	_errors;
	std::map<std::string, t_location>	_locations;
}	t_server;

class	ConfigHandler
{
private:
	std::vector<t_server>	_servers;

	bool	_is_server_block_ok(std::string &content);
	bool	_is_brackets_ok(std::string &content);
	bool	_is_valid(std::string &content);
	void	_parser(std::string &content);

public:
		ConfigHandler(void);
		ConfigHandler(std::string &content);
		~ConfigHandler(void);
};

#endif