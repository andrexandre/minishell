/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:19:13 by analexan          #+#    #+#             */
/*   Updated: 2024/01/12 12:59:01 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_builtin(t_list *curr)
{
	int		status;

	status = 1;
	if (!ft_strcmp(curr->cmds[0], "cd"))
		ms()->status = (*run_cd)(curr);
	else if (!ft_strcmp(curr->cmds[0], "echo"))
		ms()->status = (*run_echo)(curr);
	else if (!ft_strcmp(curr->cmds[0], "env"))
		ms()->status = (*prt_eplst)();
	else if (!ft_strcmp(curr->cmds[0], "export"))
		ms()->status = (*run_export)(curr);
	else if (!ft_strcmp(curr->cmds[0], "pwd"))
		ms()->status = (*run_pwd)();
	else if (!ft_strcmp(curr->cmds[0], "unset"))
		ms()->status = (*run_unset)(curr);
	else if (!ft_strcmp(curr->cmds[0], "exit"))
		ms()->status = (*run_exit)(curr);
	else
		status = 0;
	return (status);
}

void	run_cmd(t_list *curr, int j)
{
	if ((!ms()->words->next && curr->type == BUILT_IN))
	{
		if (ms()->fd[0] && dup2(ms()->fd[0], STDIN_FILENO) < 0)
			return (perror("dup2, fd[0]"));
		if (ms()->fd[1] && dup2(ms()->fd[1], STDOUT_FILENO) < 0)
			return (perror("dup2, fd[1]"));
		run_builtin(curr);
		if (ms()->fd[0] && dup2(ms()->saved_fd[0], STDIN_FILENO) < 0)
			return (perror("dup2, saved_fd[0]"));
		if (ms()->fd[1] && dup2(ms()->saved_fd[1], STDOUT_FILENO) < 0)
			return (perror("dup2, saved_fd[1]"));
	}
	else
	{
		signal(SIGINT, tmp_handler);
		signal(SIGQUIT, tmp_handler);
		ms()->pid[j] = fork();
		if (ms()->pid[j] < 0)
		{
			close_pipes(-1);
			free_all(EXIT_FAILURE, "fork");
		}
		else if (!ms()->pid[j])
			run_child(ep_from_epl(), curr);
	}
}
		// signal(SIGINT, SIG_IGN);
		// signal(SIGQUIT, SIG_IGN);

/* error:
0 -> sem erros/continuar
1 -> não executar esse pipe
2 -> não executar mais nada
*/
void	execution_loop(int len, int error, int i)
{
	t_list	*curr;

	curr = ms()->words;
	while (curr)
	{
		error = 0;
		ms()->fd[0] = 0;
		ms()->fd[1] = 0;
		if (i)
			ms()->fd[0] = ms()->pipe[i - 1][0];
		if (i != len - 1)
			ms()->fd[1] = ms()->pipe[i][1];
		redirects(curr, &error);
		if (error == 2)
			break ;
		curr->type = _is_builtin(curr->cmds[0]);
		if (!error)
			run_cmd(curr, i);
		if (ms()->fd[0])
			close(ms()->fd[0]);
		if (ms()->fd[1])
			close(ms()->fd[1]);
		curr = curr->next;
		i++;
	}
}

int	execution_init(void)
{
	int	i;
	int	len;

	len = ft_lstsize(ms()->words);
	if (len > 1)
	{
		(ms()->pipe) = ft_calloc(len - 1, sizeof(int *));
		if (!ms()->pipe)
			free_all(EXIT_FAILURE, "calloc");
		i = 0;
		while (i < len - 1)
		{
			(ms()->pipe[i]) = ft_calloc(2, sizeof(int));
			if (!ms()->pipe[i] || pipe(ms()->pipe[i]) < 0)
			{
				close_pipes(i);
				free_all(EXIT_FAILURE, "calloc/pipe");
			}
			i++;
		}
	}
	(ms()->pid) = ft_calloc(len, sizeof(int));
	if (!ms()->pid)
		free_all(EXIT_FAILURE, "calloc");
	return (len);
}

void	execution(void)
{
	int		i;
	int		len;
	int		stat;

	len = execution_init();
	execution_loop(len, 0, 0);
	stat = 0;
	i = -1;
	while (++i < len)
	{
		if (ms()->pid[i])
		{
			waitpid(ms()->pid[i], &stat, 0);
			if (WIFEXITED(stat))
				ms()->status = WEXITSTATUS(stat);
		}
	}
}
