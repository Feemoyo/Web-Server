#ifndef TOOLKIT_HPP
# define TOOLKIT_HPP

# include "Utils.hpp"

class ToolKit
{
	private:
		std::string				_content;
		std::string				_file_path;
		std::string				_file_name;

		static std::string		_status_code;

		void					_extract_content(void);
		
	public:

							ToolKit(void);
							// ToolKit(ToolKit const & src);
		virtual 			~ToolKit(void);

		// ToolKit 			&operator=(ToolKit const & rhs);

		std::string			&get_content(void);
		std::string			&get_file_path(void);
		std::string			&get_file_name(void);
		std::string			&get_status_code(void);



		void				set_file(std::string path, std::string name);
		void				set_file(std::string path_and_name);
		void				set_status_code(std::string code);
		void				status_code_handler(std::string status_code, std::string html_path);

};

// std::ostream &			operator<<( std::ostream & o, ToolKit const & i );

#endif