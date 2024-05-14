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

					Config(void);

	void			_copy_content(std::string &content);
	void			_remove_comments(void);
	void			_remove_white_spaces(void);
	bool			_verify_brackets(void);
	int				_count_occurrences(const std::string &str, const char c);
	void			_find_total_servers(void);

	void			_server_block(void);
	void			_find_config_port(t_server &server, size_t start);
	void			_find_config_server_name(t_server &server, size_t start);
	void			_find_config_root(t_server &server, size_t start);
	void			_find_config_max_body_size(t_server &server, size_t start);
	void			_find_config_errors_location(t_server &server, size_t start);
	void			_find_config_default_index_location(t_server &server, size_t start);
	void			_find_other_locations(t_server &server, size_t start);

public:
					Config(std::string &content);
					~Config(void);

	std::vector<t_server>	&get_servers(void);
	t_server				&get_server(int index);
	size_t					get_total_servers(void);
	std::string				&get_content(void);

};

std::ostream	&operator<<(std::ostream &lhs, const t_location &rhs);
std::ostream	&operator<<(std::ostream &lhs, const t_server &rhs);

#endif