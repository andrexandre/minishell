/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/12/01 19:01:54 by analexan         ###   ########.fr       */
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

int	run_builtin(void)
{
	if (!ft_strcmp(var()->words->cmds[0], "cd"))
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
	else
		return (0);
	return (1);
}

char	**remove_items(char **strs, int n)
{
	char	**new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (strs[i])
		i++;
	if (n < 0 || n >= i - 1)
		return (strs);
	new = ft_calloc(i - 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		if (i != n && i != n + 1)
			new[j++] = ft_strdup(strs[i]);
		i++;
	}
	new[j] = NULL;
	free_strs(strs);
	return (new);
}

int	reindirects_pipes(void)
{
	char	**cmds;
	int		i;

	/*
	gets as input the last </<<
	gives output to the last >/>>
	Execution "tree" WIP:
	redirect from the last redirected file

	redirect to the last redirected file
	*/
	cmds = var()->words->cmds;
	i = -1;
	int needs_dup[2];
	needs_dup[0] = 0;
	needs_dup[1] = 0;
	while (cmds[++i])
	{
		if ((!ft_strcmp(cmds[i], "<") || !ft_strcmp(cmds[i], "<<")
			|| !ft_strcmp(cmds[i], ">") || !ft_strcmp(cmds[i], ">>")) && !cmds[i + 1])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			var()->status = 2;
			return (1);
		}
		if (var()->fd[0] && cmds[i][0] == '<')
			close(var()->fd[0]);
		if (var()->fd[1] && cmds[i][0] == '>')
			close(var()->fd[1]);
		if (!ft_strcmp(cmds[i], "<"))
			var()->fd[0] = open(cmds[i + 1], O_RDONLY);
		// else if (!ft_strcmp(cmds[i], "<<"))
		// 	var()->fd[0] = get_stdin(cmds[i + 1]);
		else if (!ft_strcmp(cmds[i], ">"))
			var()->fd[1] = open(cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (!ft_strcmp(cmds[i], ">>"))
			var()->fd[1] = open(cmds[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			continue ;
		if (cmds[i][0] == '<')
		{
			if (var()->fd[0] < 0)
			{
				perror(cmds[i + 1]);
				var()->fd[0] = 0;
				return (1);
			}
			if (ft_strlen_matrix(cmds) > 2)
				needs_dup[0] = 1;
			else
			{
				close(var()->fd[0]);
				var()->fd[0] = 0;
				return (1);
			}
			var()->words->cmds = remove_items(cmds, i);
			cmds = var()->words->cmds;
			i--;
		}
		else if (cmds[i][0] == '>')
		{
			if (var()->fd[1] < 0)
			{
				perror(cmds[i + 1]);
				var()->fd[1] = 0;
				return (1);
			}
			if (ft_strlen_matrix(cmds) > 2)
				needs_dup[1] = 1;
			else
			{
				close(var()->fd[1]);
				var()->fd[1] = 0;
				return (1);
			}
			var()->words->cmds = remove_items(cmds, i);
			cmds = var()->words->cmds;
			i--;
		}
	}
	if (needs_dup[0])
	{
		if (dup2(var()->fd[0], STDIN_FILENO) < 0)
			perror("dup2");
		needs_dup[0] = 0;
	}
	if (needs_dup[1])
	{
		if (dup2(var()->fd[1], STDOUT_FILENO) < 0)
			perror("dup2");
		needs_dup[1] = 0;
	}
	return (0);
}

void	execution(int *status)
{
	char	**ep;

	if (reindirects_pipes())
		return ;
	else if (run_builtin())
		return ;
	else if (!ft_strcmp(var()->words->cmds[0], "exit")
		|| !ft_strcmp(var()->words->cmds[0], "q"))
		*status = 0;
	else
	{
		ep = ep_from_epl();
		if (get_env("PATH"))
		{
			free_strs(var()->paths);
			parsing_paths(ep, -1);
		}
		cmd_execute(ep);
		free_strs(ep);
	}
}

char	*search_cmd(char *command)
{
	int		i;
	char	*cmd;

	i = -1;
	if (!ft_strchr(command, '/') && var()->paths && get_env("PATH"))
	{
		while (var()->paths[++i])
		{
			cmd = ft_strjoin(var()->paths[i], command);
			if (!access(cmd, F_OK | X_OK))
				return (cmd);
			free(cmd);
		}
	}
	else
		if (!access(command, F_OK | X_OK))
			return (ft_strdup(command));
	ft_putstr_fd("minishell: command not found\n", 2);
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
		close(var()->saved_fd[0]);
		close(var()->saved_fd[1]);
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
