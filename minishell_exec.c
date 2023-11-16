/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/16 18:52:25 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	builtin(int *status)
{
	if (!ft_strcmp(var()->words->content, "cd"))
		return (*run_cd)();
	else if (!ft_strcmp(var()->words->content, "echo"))
		return (*run_echo)();
	else if (!ft_strcmp(var()->words->content, "env"))
		return (*run_env)();
	else if (!ft_strcmp(var()->words->content, "export"))
		return (*run_export)();
	else if (!ft_strcmp(var()->words->content, "pwd"))
		return (*run_pwd)();
	else if (!ft_strcmp(var()->words->content, "unset"))
		return (*run_unset)();
	else if (!ft_strcmp(var()->words->content, "exit") || !ft_strcmp(var()->words->content, "q"))
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
	// t_word *word;

	// word = var()->lstep_parsed->content;
	cmd = search_cmd(var()->words->content);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		// change the var()->words to char **cmdargs
		execve(cmd, (char *const[]){cmd, NULL}, ep);
		perror(cmd);
		// free_strs(cmdargs);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}
