#pragma once

#ifndef PARSER_HPP
# define PARSER_HPP

# include "Utils.hpp"

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