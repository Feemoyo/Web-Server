#pragma once

#ifndef	JSON_HPP
# define JSON_HPP

# include "Utils.hpp"

class	JSON :
{
private:
	std::string	_payload;

			JSON(void);
public:
			JSON(std::string payload);
			~JSON(void);

};

#endif