#pragma once

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ToolKit.hpp"

class	Config : public ToolKit
{
private:
	std::vector<t_server>	_servers;
	std::vector<size_t>		_total_servers;

	void			_config_total_servers(void);
	void			_config_servers(void);
	void			_config_locations(t_server &server, size_t start);
	void			_config_methods(std::string ref, t_location &location);

public:
					Config(void);
					Config(char *argv);
					~Config(void);

	t_server		&get_server(int index);
	size_t			get_total_servers(void);

};

#endif