/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_n_handlers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:18:09 by analexan          #+#    #+#             */
/*   Updated: 2024/01/11 14:22:31 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
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

void	tmp_handler(int sig)
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
		close(ms()->hd_fd);
		close(ms()->fd[1]);
		close(ms()->fd[0]);
		free_all(128 + sig, 0);
	}
}

// change to a pipe
void	get_stdin(const char *arg)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, hd_handler);
	(ms()->hd_fd) = open("/tmp/msh-hd", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (ms()->hd_fd < 0)
		free_all(EXIT_FAILURE, "open");
	ms()->hd_buf = readline("> ");
	while (ms()->hd_buf && ft_strcmp(ms()->hd_buf, arg))
	{
		ms()->hd_buf = expander(ms()->hd_buf);
		dprt(ms()->hd_fd, "%s\n", ms()->hd_buf);
		free(ms()->hd_buf);
		ms()->hd_buf = NULL;
		ms()->hd_buf = readline("> ");
	}
	if (!ms()->hd_buf)
		dprt(2, "minishell: warning: end-of-file (wanted `%s')\n", arg);
	close(ms()->hd_fd);
	close(ms()->fd[1]);
	close(ms()->fd[0]);
	free(ms()->hd_buf);
	free_all(EXIT_SUCCESS, 0);
}

// receive input from the stdin and save in in the file
int	heredoc(char *arg)
{
	int		pid;
	int		stat;

	pid = fork();
	if (pid < 0)
		free_all(EXIT_FAILURE, "fork");
	if (pid > 0)
		signal(SIGINT, tmp_hd_handler);
	if (!pid)
		get_stdin(arg);
	if (pid > 0 && waitpid(pid, &stat, 0) > 0)
		if (WIFEXITED(stat))
			ms()->status = WEXITSTATUS(stat);
	if (ms()->status == 130)
		return (-1);
	(ms()->hd_fd) = open("/tmp/msh-hd", O_RDONLY);
	if (ms()->hd_fd < 0)
		free_all(EXIT_FAILURE, "open");
	unlink("/tmp/msh-hd");
	return (ms()->hd_fd);
}
