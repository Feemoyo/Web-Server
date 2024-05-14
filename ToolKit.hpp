#pragma once

#ifndef TOOLKIT_HPP
# define TOOLKIT_HPP

# include "Utils.hpp"

class ToolKit
{
protected:
		std::string				_content;
		std::string				_file_path;
		std::string				_file_name;

		static std::string		_status_code;

		void					_extract_content(void);
		
public:

							ToolKit(void);
		virtual 			~ToolKit(void);

		std::string			&get_content(void);
		std::string			&get_file_name(void);
		std::string			&get_file_path(void);
		std::string			&get_status_code(void);

		void				set_file(std::string path, std::string name);
		void				set_file(std::string path_and_name);
		void				set_status_code(std::string code);

};

#endif
