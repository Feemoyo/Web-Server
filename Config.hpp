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

					Config(void);

	bool			_config_total_servers(void);
	bool			_config_servers(void);
	
	bool			_is_there_a_valid_port(std::string &serv, int &port);
	void			_define_visibility(std::string &serv, bool &dir);
	void			_define_max_body_size(std::string &serv, int &max_body_size);
	bool			_define_locations(std::string &serv, t_server &server);
	void			_define_methods(std::string ref, t_location &location);

public:
					Config(char *argv);
					~Config(void);

	t_server		&get_server(int index);
	size_t			get_total_servers(void);
	bool			&is_good_to_run(void);

};

#endif