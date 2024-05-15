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
					Client(char *buffer);
					~Client(void);

	void			set_buffer(char *buffer);
	
	// temos espaço para um getter de todas infos de um request ex: Content-Type.
	std::string		get_path(void);
	void			print_map(void);

};

#endif
