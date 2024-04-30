#pragma once

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>

# include <cstring>

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

class	Parser
{
private:
	std::vector<t_server>	_servers;

	bool					_is_server_block_ok(std::string &content);
	bool					_is_brackets_ok(std::string &content);
	bool					_is_valid(std::string &content);
	void					_parser(std::string &content);

public:
			Parser(void);
			Parser(std::string &content);
			~Parser(void);
};

#endif