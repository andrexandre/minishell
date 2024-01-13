/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_n_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:18:09 by analexan          #+#    #+#             */
/*   Updated: 2024/01/13 11:49:59 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_handler(int sig)
{
	if (sig == SIGINT)
	{
		prt("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	ms()->status = 128 + sig;
}

void	child_handler(int sig)
{
	if (sig == SIGQUIT)
		dprt(2, "Quit (core dumped)\n");
	if (sig == SIGINT)
		prt("\n");
	ms()->status = 128 + sig;
}

void	hd_handler(int sig)
{
	if (sig == SIGINT)
	{
		free(ms()->hd_buf);
		close(ms()->hd_fd[0]);
		close(ms()->hd_fd[1]);
		free_all(128 + sig, 0);
	}
}

// receive input from the stdin and save in in the pipe
void	get_stdin(const char *arg)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, hd_handler);
	close(ms()->hd_fd[0]);
	ms()->hd_buf = readline("> ");
	while (ms()->hd_buf && ft_strcmp(ms()->hd_buf, arg))
	{
		ms()->hd_buf = expander(ms()->hd_buf);
		dprt(ms()->hd_fd[1], "%s\n", ms()->hd_buf);
		free(ms()->hd_buf);
		ms()->hd_buf = NULL;
		ms()->hd_buf = readline("> ");
	}
	if (!ms()->hd_buf)
		dprt(2, "minishell: warning: end-of-file (wanted `%s')\n", arg);
	close(ms()->hd_fd[1]);
	free(ms()->hd_buf);
	ms()->hd_buf = NULL;
	free_all(EXIT_SUCCESS, 0);
}

int	heredoc(char *arg)
{
	int		pid;
	int		stat;

	if (pipe(ms()->hd_fd) < 0)
		free_all(EXIT_FAILURE, "pipe");
	pid = fork();
	if (pid < 0)
		free_all(EXIT_FAILURE, "fork");
	if (pid > 0)
		signal(SIGINT, parent_hd_handler);
	if (!pid)
		get_stdin(arg);
	if (waitpid(pid, &stat, 0) > 0)
		if (WIFEXITED(stat))
			ms()->status = WEXITSTATUS(stat);
	if (ms()->status == 130)
		return (-1);
	close(ms()->hd_fd[1]);
	return (ms()->hd_fd[0]);
}
