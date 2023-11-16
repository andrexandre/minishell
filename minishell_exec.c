/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/16 15:41:24 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	builtin(int *status)
{
	t_word *word;
	char **cmdargs;
        word = var()->lstep_parsed->content
	cmdargs = word->cmds;
	if (!ft_strcmp(cmdargs[0], "cd"))
		return (*run_cd)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "echo"))
		return (*run_echo)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "env"))
		return (*run_env)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "export"))
		return (*run_export)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "pwd"))
		return (*run_pwd)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "unset"))
		return (*run_unset)(cmdargs);
	else if (!ft_strcmp(cmdargs[0], "exit") || !ft_strcmp(cmdargs[0], "q"))
		*status = 0;
	return (*status);
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

void	cmd_execute(char **ep)
{
	char	*cmd;
	int		pid;
	t_word *word;

	word = var()->lstep_parsed->content;
	cmd = search_cmd(word->cmds, word->cmds[0]);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		execve(cmd, word->cmds, ep);
		perror(word->cmds[0]);
		free_strs(word->cmds);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}
