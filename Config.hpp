#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ToolKit.hpp"

class	Config : public ToolKit
{
private:
	std::vector<t_server>	_servers;
	std::vector<size_t>		_total_servers;

	void			_remove_comments(void);
	void			_remove_white_spaces(void);
	bool			_verify_brackets(void);
	int				_count_occurrences(const std::string &str, const char c);
	void			_find_total_servers(void);

	void			_server_block(void);
	std::string		_find_in_config_file(size_t start, std::string ref_start, std::string ref_end);
	void			_find_config_errors_location(t_server &server, size_t start);
	void			_find_config_default_index_location(t_server &server, size_t start);
	void			_find_other_locations(t_server &server, size_t start);

public:
							Config(void);
							Config(char *argv);
							~Config(void);

	std::vector<t_server>	&get_servers(void);
	t_server				&get_server(int index);
	size_t					get_total_servers(void);
	std::string				&get_content(void);

};

#endif