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

	void			request_process(int &client_fd);

	std::string		get_path(void);
	void			print_map(void);

};

#endif
