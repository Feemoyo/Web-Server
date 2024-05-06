#include "ToolKit.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ToolKit::ToolKit()
{
}

// ToolKit::ToolKit( const ToolKit & src )
// {
// }


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ToolKit::~ToolKit()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

// ToolKit	&ToolKit::operator=( ToolKit const & rhs )
// {
// 	//if ( this != &rhs )
// 	//{
// 		//this->_value = rhs.getValue();
// 	//}
// 	return *this;
// }

// std::ostream	&operator<<( std::ostream & o, ToolKit const & i )
// {
// 	//o << "Value = " << i.getValue();
// 	return o;
// }


/*
** --------------------------------- METHODS ----------------------------------
*/

void	ToolKit::_extract_content(void)
{
	std::ifstream	file((this->_file_path + this->_file_name).c_str());
	std::string		line;

	if (!file.is_open())
	{
		std::cerr << "File cannot be opened" << std::endl;
		return ;
	}
	if (file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << "File is empty" << std::endl;
		file.close();
		return ;
	}
	while (std::getline(file, line))
		this->_content += line + '\n';
	file.close();
	return ;
}

void	ToolKit::status_code_handler(std::string status_code, std::string html_path)
{
	std::map<std::string, std::string> statusCodes;

	statusCodes.insert(std::make_pair("100", "Continue"));
	statusCodes.insert(std::make_pair("101", "Switching Protocols"));
	statusCodes.insert(std::make_pair("102", "Processing"));
	statusCodes.insert(std::make_pair("200", "OK"));
	statusCodes.insert(std::make_pair("201","Created"));
	statusCodes.insert(std::make_pair("202", "Accepted"));
	statusCodes.insert(std::make_pair("203", "Non-Authoritative Information"));
	statusCodes.insert(std::make_pair("204",  "No Content"));
	statusCodes.insert(std::make_pair("205", "Reset Content"));
	statusCodes.insert(std::make_pair("206", "Partial Content"));
	["207"] = "Multi-Status";
	["208"] = "Already Reported";
	["226"] = "IM Used";
	["300"] = "Multiple Choices";
	["301"] = "Moved Permanently";
	["302"] = "Found";
	["303"] = "See Other";
	["304"] = "Not Modified";
	["305"] = "Use Proxy";
	["307"] = "Temporary Redirect";
	["308"] = "Permanent Redirect";
	["400"] = "Bad Request";
	["401"] = "Unauthorized";
	["402"] = "Payment Required";
	["403"] = "Forbidden";
	["404"] = "Not Found";
	["405"] = "Method Not Allowed";
	["406"] = "Not Acceptable";
	["407"] = "Proxy Authentication Required";
	["408"] = "Request Timeout";
	["409"] = "Conflict";
	["410"] = "Gone";
	["411"] = "Length Required";
	["412"] = "Precondition Failed";
	["413"] = "Payload Too Large";
	["414"] = "URI Too Long";
	["415"] = "Unsupported Media Type";
	["416"] = "Range Not Satisfiable";
	["417"] = "Expectation Failed";
	["418"] = "I'm a teapot"
	["421"] = "Misdirected Request";
	["422"] = "Unprocessable Entity";
	["423"] = "Locked";
	["424"] = "Failed Dependency";
	["425"] = "Too Early";
	["426"] = "Upgrade Required";
	["428"] = "Precondition Required";
	["429"] = "Too Many Requests";
	["431"] = "Request Header Fields Too Large";
	["451"] = "Unavailable For Legal Reasons";
	["500"] = "Internal Server Error";
	["501"] = "Not Implemented";
	["502"] = "Bad Gateway";
	["503"] = "Service Unavailable";
	["504"] = "Gateway Timeout";
	["505"] = "HTTP Version Not Supported";
	["506"] = "Variant Also Negotiates";
	["507"] = "Insufficient Storage";
	["508"] = "Loop Detected";
	["510"] = "Not Extended";
	["511"] = "Network Authentication Required";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	&ToolKit::get_content(void)
{
	return (this->_content);
}

std::string	&ToolKit::get_file_name(void)
{
	return (this->_file_name);
}

std::string	&ToolKit::get_file_path(void)
{
	return (this->_file_path);
}

std::string	&ToolKit::get_status_code(void)
{
	return (this->_status_code);
}

void	ToolKit::set_file(std::string path, std::string name)
{
	this->_file_path = path;
	this->_file_name = name;
	return ;
}

void	ToolKit::set_file(std::string path_and_name)
{
	this->_file_path = path_and_name.substr(0, path_and_name.find_last_of('/') + 1);
	this->_file_name = path_and_name.substr(path_and_name.find_last_of('/') + 1);
	return ;
}

void	ToolKit::set_status_code(std::string code)
{
	this->_status_code = code;
	return ;
}

/* ************************************************************************** */