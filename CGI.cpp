/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferrero <rferrero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:35:31 by rferrero          #+#    #+#             */
/*   Updated: 2024/07/30 11:51:57 by rferrero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(void)
{
	return ;
}

CGI::CGI(std::string &method, std::string &script, std::string &html, std::string &data)
{
	this->_env["METHOD"] = method;
	this->_env["SCRIPT"] = script;
	this->_env["HTML"] = html;

	if (method == "POST" || method == "DELETE")
		this->_env["DATA"] = data;
	else
		this->_env["DATA"] = "";

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

		//	input do cgi
		char	**envp = this->_conver_map_to_array(this->_env);
		char	*argv[] = {const_cast<char *>(this->_script_path.c_str()), NULL};

		if (execve(this->_script_path.c_str(), argv, envp) == -1)
		{
			std::cerr << "Execve fail to run" << "\n";
			// exit(EXIT_FAILURE);
			kill(getpid(), SIGTERM);
		}
	}
	else
	{
		close(pipe_out[1]);

		//	output do cgi
		char		buffer[1024];
		std::string	cig_output;
		size_t		bytes_read;

		while((bytes_read = read(pipe_out[0], buffer, sizeof(buffer))) > 0)
		{
			cig_output.append(buffer, bytes_read);
		}

		close(pipe_out[0]);

		int	status;
		waitpid(pid, &status, 0);

		std::cout << "Output: " << cig_output << "\n";
	}
	return ;
}
