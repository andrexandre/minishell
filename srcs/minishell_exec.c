/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 15:07:09 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(int *status)
{
	if (!var()->lst_lexer)
		return (0);
	else if (!ft_strcmp(var()->lst_lexer->str, "cd"))
		var()->status = (*run_cd)();
	else if (!ft_strcmp(var()->lst_lexer->str, "echo"))
		var()->status = (*run_echo)();
	else if (!ft_strcmp(var()->lst_lexer->str, "env"))
		var()->status = (*run_env)();
	else if (!ft_strcmp(var()->lst_lexer->str, "export"))
		var()->status = (*run_export)();
	else if (!ft_strcmp(var()->lst_lexer->str, "pwd"))
		var()->status = (*run_pwd)();
	else if (!ft_strcmp(var()->lst_lexer->str, "unset"))
		var()->status = (*run_unset)();
	else if (!ft_strcmp(var()->lst_lexer->str, "exit")
		|| !ft_strcmp(var()->lst_lexer->str, "q"))
		*status = 0;
	else
		return (1);
	return (0);
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
	var()->status = 127;
	return (NULL);
}

void	cmd_execute(char **ep)
{
	char	*cmd;
	int		pid;

	cmd = search_cmd(var()->lst_lexer->str);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		execve(cmd, var()->cmdargs, ep);
		perror(cmd);
		free_strs(var()->cmdargs);
		free_all();
		exit(126);
	}
	wait(&pid);
	if (WIFEXITED(pid))
		var()->status = WEXITSTATUS(pid);
	else
		var()->status = 130;
	free(cmd);
}
