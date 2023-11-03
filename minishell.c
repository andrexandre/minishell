/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/03 18:58:22 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	builtin(char *buf, int *status)
{
	if (!ft_strncmp(buf, "\0", 1))
		return (1);
	if (!ft_strncmp(buf, "cd\0", 3))
		return (1);
	if (!ft_strncmp(buf, "echo\0", 5) || !ft_strncmp(buf, "echo -n\0", 8))
		return (1);
	if (!ft_strncmp(buf, "env\0", 4))
		return (1);
	if (!ft_strncmp(buf, "export\0", 7))
		return (1);
	if (!ft_strncmp(buf, "pwd\0", 4))
		return (1);
	if (!ft_strncmp(buf, "unset\0", 6))
		return (1);
	if (!ft_strncmp(buf, "exit\0", 5) || !ft_strncmp(buf, "q\0", 2))
	{
		*status = 0;
		return (1);
	}
	return (0);
}

void	cmd_loop(char **ep)
{
	char	**cmdargs;
	char	*buf;
	int		status;

	status = 1;
	while (status)
	{
		prt("minishell> ");
		buf = get_next_line(0);
		buf[ft_strlen(buf) - 1] = '\0';
		cmdargs = ft_split(buf, ' ');
		if (!builtin(buf, &status))
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
