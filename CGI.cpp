/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:35:31 by rferrero          #+#    #+#             */
/*   Updated: 2024/08/13 13:17:38 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(void)
{
	return ;
}

CGI::CGI(t_cgi &cgi)
{
	this->_env["METHOD"] = cgi.method;
	this->_env["CGI"] = cgi.cgi;
	this->_env["SCRIPT_FILE"] = cgi.script_file;
	this->_env["DATA"] = cgi.data;

	return ;
}

CGI::~CGI(void)
{
	return ;
}

char	**CGI::_conver_map_to_array(std::map<std::string, std::string> &env)
{
	char	**env_arr = new char *[env.size() + 1];
	int		i = 0;

	for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++)
	{
		std::string	entry = it->first + "=" + it->second;
		env_arr[i] = new char[entry.size() + 1];
		std::strcpy(env_arr[i], entry.c_str());
		++i;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

void	CGI::run_cgi(void)
{
	int	pipe_out[2];
	if (pipe(pipe_out) == -1)
	{
		std::cerr << "Pipe fail to execute" << "\n";
		return ;
	}

	pid_t	pid = fork();
	if (pid == -1)
	{
		std::cerr << "Fork fail to execute" << "\n";
		return ;
	}
	else if (pid == 0)
	{
		close(pipe_out[0]);

		dup2(pipe_out[1], STDOUT_FILENO);

		char	**envp = this->_conver_map_to_array(this->_env);
		char	*argv[] = {const_cast<char *>(this->_env["CGI"].c_str()), const_cast<char *>(this->_env["SCRIPT_FILE"].c_str()), NULL};

		if (execve(this->_env["CGI"].c_str(), argv, envp) == -1)
		{
			std::cerr << "Execve fail to run" << "\n";
			kill(getpid(), SIGTERM);
		}
	}
	else
	{
		close(pipe_out[1]);

		char		buffer[1024];
		size_t		bytes_read;

		while((bytes_read = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
			this->_cgi_ret.append(buffer, bytes_read);

		close(pipe_out[0]);

		int	status;
		waitpid(pid, &status, 0);
	}
	return ;
}

std::string	CGI::get_cgi_ret(void)
{
	return (this->_cgi_ret);
}
