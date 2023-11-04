/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/04 17:37:44 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prt_strs(char **strs)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		prt("%s", strs[i]);
		if (strs[i + 1])
			prt(" ");
	}
}

void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return (NULL);
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (NULL);
}

void	free_all(void)
{
	free_strs(var()->paths);
}

char	*search_cmd(char **cmdargs, char *cmd)
{
	int		i;

	i = -1;
	if (!ft_strchr(cmdargs[0], '/'))
	{
		if (var()->paths)
		{
			while (var()->paths[++i])
			{
				cmd = ft_strjoin(var()->paths[i], cmdargs[0]);
				if (!access(cmd, F_OK | X_OK))
					return (cmd);
				free(cmd);
			}
		}
	}
	else
		if (!access(cmdargs[0], F_OK | X_OK))
			return (ft_strdup(cmdargs[0]));
	perror(cmdargs[0]);
	return (NULL);
}

void	cmd_execute(char **cmdargs, char **ep)
{
	char	*cmd;
	int		pid;

	cmd = search_cmd(cmdargs, cmdargs[0]);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		execve(cmd, cmdargs, ep);
		perror(cmdargs[0]);
		free_strs(cmdargs);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}

// to-do: separate builtins to functions
int	builtin(char **cmdargs, int *status)
{
	char	*temp;

	if (!cmdargs[0])
		return (1);
	else if (!ft_strcmp(cmdargs[0], "cd"))
	{
		temp = ft_strjoin("./", cmdargs[0]);
		if (!cmdargs[1])
			chdir(getenv("HOME"));
		else
			if (chdir(cmdargs[1]) < 0)
				if (chdir(temp) < 0)
					perror(cmdargs[1]);
		free(temp);
		prt("%s\n", getcwd(NULL, 0)); // debug
		return (1);
	}
	else if (!ft_strcmp(cmdargs[0], "echo"))
	{
		if (!ft_strncmp(cmdargs[1], "-n", 2))
			prt_strs(cmdargs + 2);
		else
		{
			prt_strs(cmdargs + 1);
			prt("\n");
		}
		return (1);
	}
	else if (!ft_strcmp(cmdargs[0], "env"))
		return (1);
	else if (!ft_strcmp(cmdargs[0], "export"))
		return (1);
	else if (!ft_strcmp(cmdargs[0], "pwd"))
	{
		prt("%s\n", getcwd(NULL, 0));
		return (1);
	}
	else if (!ft_strcmp(cmdargs[0], "unset"))
		return (1);
	else if (!ft_strcmp(cmdargs[0], "exit") || !ft_strcmp(cmdargs[0], "q"))
	{
		*status = 0;
		return (1);
	}
	return (0);
}


/* searching for commands:
if (has slashes)
	execute it direcly;
else if (its a builtin)
	execute builtin;
else
	search for it in PATH;
*/
void	cmd_loop(char **ep)
{
	char	**cmdargs;
	char	*buf;
	int		status;

	status = 1;
	while (status)
	{
		prt("\033[0;34mminishell\033[0m$ ");
		buf = get_next_line(0);
		buf[ft_strlen(buf) - 1] = '\0';
		cmdargs = ft_split(buf, ' ');
		if (!builtin(cmdargs, &status))
			cmd_execute(cmdargs, ep);
		free(buf);
		free_strs(cmdargs);
	}
}

void	parsing_paths(char **ep, int i)
{
	char	*path_from_ep;
	char	*temp;

	path_from_ep = NULL;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=", 5);
		if (path_from_ep)
			break ;
	}
	if (!path_from_ep)
		return ;
	path_from_ep += 5;
	var()->paths = ft_split(path_from_ep, ':');
	if (!var()->paths)
		exit(EXIT_FAILURE);
	i = -1;
	while (var()->paths[++i])
	{
		temp = var()->paths[i];
		(var()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!var()->paths[i])
			exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av, char **ep)
{
	// load
	var()->ac = ac;
	var()->av = av;
	parsing_paths(ep, -1);
	// loop
	cmd_loop(ep);
	// clean
	free_all();
	return (0);
}
