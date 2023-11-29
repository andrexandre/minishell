/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 19:03:09 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ep_from_epl(void)
{
	t_eplist	*curr;
	char		**ep;
	int			i;
	
	curr = var()->epl;
	i = 0;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	ep = ft_calloc(i + 1, sizeof(char *));
	if (!ep)
		return (NULL);
	i = 0;
	curr = var()->epl;
	while (curr)
	{
		ep[i] = ft_strdup(curr->str);
		curr = curr->next;
		i++;
	}
	ep[i] = NULL;
	return (ep);
}

void	execution(int *status)
{
	char	**ep;

	if (!var()->words)
		return ;
	else if (!ft_strcmp(var()->words->cmds[0], "cd"))
		var()->status = (*run_cd)();
	else if (!ft_strcmp(var()->words->cmds[0], "echo"))
		var()->status = (*run_echo)();
	else if (!ft_strcmp(var()->words->cmds[0], "env"))
		var()->status = (*run_env)();
	else if (!ft_strcmp(var()->words->cmds[0], "export"))
		var()->status = (*run_export)();
	else if (!ft_strcmp(var()->words->cmds[0], "pwd"))
		var()->status = (*run_pwd)();
	else if (!ft_strcmp(var()->words->cmds[0], "unset"))
		var()->status = (*run_unset)();
	else if (!ft_strcmp(var()->words->cmds[0], "exit")
		|| !ft_strcmp(var()->words->cmds[0], "q"))
		*status = 0;
	else
	{
		ep = ep_from_epl();
		cmd_execute(ep);
		free_strs(ep);
	}
}

char	*search_cmd(char *command)
{
	int		i;
	char	*cmd;

	i = -1;
	if (!ft_strchr(command, '/'))
	{
		if (var()->paths)
		{
			while (var()->paths[++i])
			{
				cmd = ft_strjoin(var()->paths[i], command);
				if (!access(cmd, F_OK | X_OK))
					return (cmd);
				free(cmd);
			}
		}
	}
	else
		if (!access(command, F_OK | X_OK))
			return (ft_strdup(command));
	perror(command);
	var()->status = 127;
	return (NULL);
}

void	tmp_handler(int sig)
{
	(void)sig;
	prt("\n");
}

void	cmd_execute(char **ep)
{
	char	*cmd;
	int		pid;

	cmd = search_cmd(var()->words->cmds[0]);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	signal(SIGINT, tmp_handler);
	signal(SIGQUIT, handler);
	if (!pid)
	{
		execve(cmd, var()->words->cmds, ep);
		perror(cmd);
		ft_lstclear(&var()->words, free_lst);
		free_strs(ep);
		free_all(126);
	}
	wait(&pid);
	if (WIFEXITED(pid))
		var()->status = WEXITSTATUS(pid);
	else
		var()->status = 130;
	free(cmd);
}
