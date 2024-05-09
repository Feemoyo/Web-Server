#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Utils.hpp"

class	Config
{
private:
	std::vector<t_server>	_servers;
	size_t					_total_servers;
	std::string				_content;

	void					_copy_content(std::string &content);
	void					_remove_comments(void);
	void					_remove_white_spaces(void);
	void					_server_block(void);

public:
							Config(void);
							Config(std::string &content);
							~Config(void);

	// std::vector<t_server>	&get_servers(void);
	// t_server				&get_server(int index);
	size_t					&get_total_servers(void);
	std::string				&get_content(void);

	// void					set_server(t_server server_config);

};

#endif
