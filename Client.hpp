#pragma once

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ToolKit.hpp"
# include "Mime.hpp"
# include "JSON.hpp"

class	Client : public ToolKit
{
private:
	std::map<std::string, std::string>	_buffer_map;
	Mime								_mime;

	std::string							_map_finder(std::string key, std::string value1, std::string value2);
	void								_request_header(std::istringstream &stream);

public:
					Client(void);
					~Client(void);

	bool			set_buffer(std::vector<char> buffer, bool &payload);
	void			run_json(std::string &root);

	std::string		get_method(void);
	std::string		get_path(void);
	std::string		get_map_content(std::string ref);

	void			format_content_type(void);
	void			format_payload(void);
	void			set_body_size(void);

	void			clear_buffer(void);
	void			clear_body_size(void);

	void			print_map(void);

};

#endif
