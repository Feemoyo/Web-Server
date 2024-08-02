#pragma once

#ifndef	JSON_HPP
# define JSON_HPP

# include "ToolKit.hpp"

class	JSON : public ToolKit
{
private:
	std::string	_payload;
	std::string	_path;
	std::string	_file;

			JSON(void);

	void	_json_delete(int comment);
	void	_json_post(void);

	void	_payload_parser(void);
	void	_replace_ampersand(void);
	void	_replace_equal(void);

public:
			JSON(std::string payload, std::string path, std::string file);
			~JSON(void);

	void	run(void);

};

#endif