/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/23 22:09:40 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(int *status)
{
	if (!var()->words)
		return (0);
	else if (!ft_strcmp(var()->words->str, "cd"))
		return (*run_cd)();
	else if (!ft_strcmp(var()->words->str, "echo"))
		return (*run_echo)();
	else if (!ft_strcmp(var()->words->str, "env"))
		return (*run_env)();
	else if (!ft_strcmp(var()->words->str, "export"))
		return (*run_export)();
	else if (!ft_strcmp(var()->words->str, "pwd"))
		return (*run_pwd)();
	else if (!ft_strcmp(var()->words->str, "unset"))
		return (*run_unset)();
	else if (!ft_strcmp(var()->words->str, "exit") || !ft_strcmp(var()->words->str, "q"))
		*status = 0;
	return (*status);
}

char	*search_cmd(char *cmdargs)
{
	int		i;
	char	*cmd;

	i = -1;
	if (!ft_strchr(cmdargs, '/'))
	{
		if (var()->paths)
		{
			while (var()->paths[++i])
			{
				cmd = ft_strjoin(var()->paths[i], cmdargs);
				if (!access(cmd, F_OK | X_OK))
					return (cmd);
				free(cmd);
			}
		}
	}
	else
		if (!access(cmdargs, F_OK | X_OK))
			return (ft_strdup(cmdargs));
	perror(cmdargs);
	return (NULL);
}

void	cmd_execute(char **ep)
{
	char	*cmd;
	int		pid;

	cmd = search_cmd(var()->words->str);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		// give the char **currcmdargs to execve
		execve(cmd, (char *const[]){cmd, NULL}, ep);
		perror(cmd);
		// free_strs(cmdargs);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}
