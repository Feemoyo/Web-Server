#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ToolKit.hpp"

class	Client : public ToolKit
{
private:
	std::map<std::string, std::string>	_buffer_map;

public:
					Client(void);
					~Client(void);


	void			set_buffer(char *buffer);
	void			print_map(void);
	std::string		get_path(void);

};

#endif
