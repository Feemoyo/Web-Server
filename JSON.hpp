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

public:
			JSON(std::string payload, std::string path, std::string file);
			~JSON(void);

	void	run(void);

};

#endif