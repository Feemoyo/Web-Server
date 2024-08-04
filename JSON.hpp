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

	void	_payload_parser(void);
	void	_replace_ampersand(void);
	void	_replace_equal(void);
	bool	_check_dir_and_file(void);

public:
			JSON(std::string payload, std::string path, std::string file);
			~JSON(void);

	void	json_delete(int comment);
	void	json_post(void);

};

#endif