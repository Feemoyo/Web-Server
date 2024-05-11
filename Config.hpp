#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Utils.hpp"

class	Config
{
private:
	std::vector<t_server>	_servers;
	std::vector<size_t>		_total_servers;
	std::string				_content;

	void			_copy_content(std::string &content);
	void			_remove_comments(void);
	void			_remove_white_spaces(void);
	void			_find_total_servers(void);

	void			_server_block(void);
	void			_find_config_port(t_server &server, size_t start);
	void			_find_config_server_name(t_server &server, size_t start);
	void			_find_config_root(t_server &server, size_t start);
	void			_find_config_max_body_size(t_server &server, size_t start);
	void			_find_config_errors_location(t_server server, size_t start);

public:
					Config(void);
					Config(std::string &content);
					~Config(void);

	// std::vector<t_server>	&get_servers(void);
	// t_server				&get_server(int index);
	// size_t					&get_total_servers(void);
	std::string				&get_content(void);

	// void					set_server(t_server server_config);

};

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs);
// {
// 	lhs << rhs.path << std::endl;
// 	lhs << rhs.default_file << std::endl;
// 	for (size_t i = 0; i < rhs.methods.size(); i++)
// 		lhs << rhs.methods[i] << std::endl;
// 	return (lhs);
// }

#endif