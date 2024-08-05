#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ToolKit.hpp"

class	Config : public	ToolKit
{
private:
	std::vector<t_server>		_servers;
	std::vector<std::string>	_server_string;
	bool						_good_to_run;

	bool			_config_total_servers(void);
	bool			_config_servers(void);
	bool			_is_there_a_valid_port(std::string &serv, int &port);
	void			_set_server_name(std::string &serv, std::string &name);
	void			_set_server_root(std::string &serv, std::string &root);
	void			_set_dir_visibility(std::string &serv, bool &dir);
	void			_set_max_body_size(std::string &serv, int &max_body_size);
	bool			_config_locations(std::string &serv, t_server &server);
	bool			_check_for_redirection(std::string &locations, size_t ref);
	void			_config_methods(std::string ref, t_location &location);

public:
					Config(void);
					Config(char *argv);
					~Config(void);

	t_server		&get_server(int index);
	size_t			get_total_servers(void);
	bool			&is_good_to_run(void);

};

#endif