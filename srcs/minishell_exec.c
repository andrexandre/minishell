/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/12/22 19:05:10 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ep_from_epl(void)
{
	t_eplist	*curr;
	char		**ep;
	int			i;
	
	curr = ms()->epl;
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
	curr = ms()->epl;
	while (curr)
	{
		ep[i] = ft_strdup(curr->str);
		curr = curr->next;
		i++;
	}
	ep[i] = NULL;
	return (ep);
}

int	run_builtin(t_list *curr)
{
	t_list	*tmp;

	tmp = ms()->words;
	ms()->words = curr;
	if (!ft_strcmp(ms()->words->cmds[0], "cd"))
		ms()->status = (*run_cd)();
	else if (!ft_strcmp(ms()->words->cmds[0], "echo"))
		ms()->status = (*run_echo)();
	else if (!ft_strcmp(ms()->words->cmds[0], "env"))
		ms()->status = (*prt_eplst)();
	else if (!ft_strcmp(ms()->words->cmds[0], "export"))
		ms()->status = (*run_export)();
	else if (!ft_strcmp(ms()->words->cmds[0], "pwd"))
		ms()->status = (*run_pwd)();
	else if (!ft_strcmp(ms()->words->cmds[0], "unset"))
		ms()->status = (*run_unset)();
	else if (!ft_strcmp(ms()->words->cmds[0], "exit")
		|| !ft_strcmp(ms()->words->cmds[0], "q"))
		ms()->status = (*run_exit)();
	else
	{
		ms()->words = tmp;
		return (0);
	}
	ms()->words = tmp;
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

void	free_pipes_words(void)
{
	int	i;
	int	len;

	i = -1;
	len = ft_lstsize(ms()->words);
	while (ms()->pipe && ++i < len - 1)
		free(ms()->pipe[i]);
	if (len > 1)
		free(ms()->pipe);
	free(ms()->pid);
	ms()->pid = NULL;
	ft_lstclear(&ms()->words, free_lst);
	ft_lstclear(&ms()->lst_lexer, free_lst);
}

void	hd_handler(int sig)
{
	if (sig == SIGINT)
	{
		free(ms()->hd_buf);
		close(ms()->hd_fd);
		free_all(128 + sig, 0);
	}
}

void	get_stdin(const char *arg)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, hd_handler);
	ms()->hd_fd = open("/tmp/msh-hd", O_WRONLY | O_CREAT | O_TRUNC , 0600);
	if (ms()->hd_fd < 0)
		free_all(EXIT_FAILURE, "open");
	ms()->hd_buf = readline("> ");
	while (ms()->hd_buf && ft_strcmp(ms()->hd_buf, arg))
	{
		write(ms()->hd_fd, ms()->hd_buf, ft_strlen(ms()->hd_buf));
		write(ms()->hd_fd, "\n", 1);
		free(ms()->hd_buf);
		ms()->hd_buf = readline("> ");
	}
	if (!ms()->hd_buf)
		prt("end-of-file (wanted `%s')\n", arg);
	close(ms()->hd_fd);
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
	else if (!pid)
		get_stdin(arg);
	if (pid > 0 && waitpid(pid, &stat, 0) > 0)
		if (WIFEXITED(stat))
			ms()->status = WEXITSTATUS(stat);
	if (ms()->status == 130)
		return (-2);
	ms()->hd_fd = open("/tmp/msh-hd", O_RDONLY);
	if (ms()->hd_fd < 0)
		free_all(EXIT_FAILURE, "open");
	unlink("/tmp/msh-hd");
	return (ms()->hd_fd);
}

void	tmp_handler(int sig)
{
	if (sig == SIGQUIT)
		prt("Quit (core dumped)\n");
	if (sig == SIGINT)
		prt("\n");
	ms()->status = 128 + sig;
}

void	execute_builtin(t_list *curr)
{
	if (ms()->fd[0] && dup2(ms()->fd[0], STDIN_FILENO) < 0)
		perror("dup2, fd[0]");
	if (ms()->fd[1] && dup2(ms()->fd[1], STDOUT_FILENO) < 0)
		perror("dup2, fd[1]");
	run_builtin(curr);
	if (ms()->fd[0] && !close(ms()->fd[0])
		&& !ms()->words->next && dup2(ms()->saved_fd[0], STDIN_FILENO) < 0)
		perror("dup2, saved_fd[0]");
	if (ms()->fd[1] && !close(ms()->fd[1])
		&& !ms()->words->next && dup2(ms()->saved_fd[1], STDOUT_FILENO) < 0)
		perror("dup2, saved_fd[1]");
}

void	execute_pipe(t_list *curr, int j)
{
	signal(SIGINT, tmp_handler);
	signal(SIGQUIT, tmp_handler);
	ms()->pid[j] = fork();
	if (ms()->pid[j] < 0)
	{
		int i = -1;
		while (ms()->pipe && ++i < ft_lstsize(ms()->words) - 1)
		{
			close(ms()->pipe[i][0]);
			close(ms()->pipe[i][1]);
		}
		free_all(EXIT_FAILURE, "fork");
	}
	else if (!ms()->pid[j])
		cmd_execute(NULL, ep_from_epl(), curr);
	if (ms()->fd[0])
		close(ms()->fd[0]);
	if (ms()->fd[1])
		close(ms()->fd[1]);
}

void	execute(t_list *curr, int j)
{
	if ((!ms()->words->next && curr->type == BUILT_IN) || !ft_strcmp(ms()->words->cmds[0], "q"))
		execute_builtin(curr);
	else
		execute_pipe(curr, j);
}

// to-do: check all errors and frees with make v
void	execution(void)
{
	t_list	*curr;
	int		i;
	int		j;
	int		len;
	int		error;

	curr = ms()->words;
	len = ft_lstsize(ms()->words);
	if (len > 1)
		ms()->pipe = ft_calloc(len - 1, sizeof(int *));
	j = 0;
	while (ms()->pipe && j < len - 1)
	{
		ms()->pipe[j] = ft_calloc(2, sizeof(int));
		if (pipe(ms()->pipe[j]) < 0)
			free_all(EXIT_FAILURE, "pipe");
		j++;
	}
	ms()->pid = ft_calloc(len, sizeof(int));
	j = 0;
	while (ms()->pid && curr)
	{
		error = 0;
		ms()->fd[0] = 0;
		ms()->fd[1] = 0;
		if (ms()->pipe && j)
			ms()->fd[0] = ms()->pipe[j - 1][0];
		if (ms()->pipe && j != len - 1)
			ms()->fd[1] = ms()->pipe[j][1];
		i = -1;
		while (curr->cmds[++i])
		{
			if ((!ft_strcmp(curr->cmds[i], "<") || !ft_strcmp(curr->cmds[i], "<<")
				|| !ft_strcmp(curr->cmds[i], ">") || !ft_strcmp(curr->cmds[i], ">>")) && !curr->cmds[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				ms()->status = 2;
				return ;
			}
			if (ms()->fd[0] && curr->cmds[i][0] == '<')
				close(ms()->fd[0]);
			if (ms()->fd[1] && curr->cmds[i][0] == '>')
				close(ms()->fd[1]);
			if (!ft_strcmp(curr->cmds[i], "<"))
				ms()->fd[0] = open(curr->cmds[i + 1], O_RDONLY);
			else if (!ft_strcmp(curr->cmds[i], "<<"))
				ms()->fd[0] = heredoc(curr->cmds[i + 1]);
			else if (!ft_strcmp(curr->cmds[i], ">"))
				ms()->fd[1] = open(curr->cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (!ft_strcmp(curr->cmds[i], ">>"))
				ms()->fd[1] = open(curr->cmds[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				continue ;
			if (curr->cmds[i][0] == '<')
			{
				if (ms()->fd[0] < 0)
				{
					if (ms()->status != 130 && ms()->fd[0] != -2)
					{
						perror(curr->cmds[i + 1]);
						ms()->status = 1;
					}
					ms()->fd[0] = 0;
					error = 2;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(ms()->fd[0]);
					ms()->fd[0] = 0;
					error = 1;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
			else if (curr->cmds[i][0] == '>')
			{
				if (ms()->fd[1] < 0)
				{
					if (ms()->status != 130)
						perror(curr->cmds[i + 1]);
					ms()->fd[1] = 0;
					error = 1;
					ms()->status = 1;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(ms()->fd[1]);
					ms()->fd[1] = 0;
					error = 1;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
		}
		if (!error)
			execute(curr, j);
		if (error == 2)
			break ;
		curr = curr->next;
		j++;
	}
	int stat = 0;
	j = -1;
	while (ms()->pid && ++j < len)
	{
		if (ms()->pid[j])
		{
			waitpid(ms()->pid[j], &stat, 0);
			if (WIFEXITED(stat))
				ms()->status = WEXITSTATUS(stat);
		}
	}
	free_pipes_words();
}

char	*search_cmd(char *command)
{
	int			i;
	char		*cmd;
	struct stat	statbuf;

	i = -1;
	if (!ft_strchr(command, '/') && ms()->paths && get_env("PATH"))
	{
		while (ms()->paths[++i])
		{
			cmd = ft_strjoin(ms()->paths[i], command);
			if (!access(cmd, F_OK | X_OK))
				return (cmd);
			free(cmd);
		}
	}
	else
	{
		if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
			dprt(2, "%s: is a directory\n", command);
		else if (!access(command, F_OK | X_OK))
			return (ft_strdup(command));
		else
		{
			perror(command);
			ms()->status = 126;
		}
		ms()->status = 127;
		return (NULL);
	}
	dprt(2, "%s: command not found\n", command);
	ms()->status = 127;
	return (NULL);
}

void	cmd_execute(char *cmd, char **ep, t_list *curr)
{
	int	i;

	// cat echo (execve failed) bcs it detects cat echo as a built-in
	if (curr->type != BUILT_IN)
		cmd = search_cmd(curr->cmds[0]);
	parsing_paths();
	if (ms()->fd[0] && dup2(ms()->fd[0], STDIN_FILENO) < 0)
		perror("dup2, fd[0]");
	if (ms()->fd[1] && dup2(ms()->fd[1], STDOUT_FILENO) < 0)
		perror("dup2, fd[1]");
	i = -1;
	while (++i < ft_lstsize(ms()->words) - 1)
	{
		close(ms()->pipe[i][0]);
		close(ms()->pipe[i][1]);
	}
	if (curr->type != BUILT_IN && !cmd)
	{
		free_strs(ep);
		free_all(ms()->status, 0);
	}
	if (!run_builtin(curr))
	{
		execve(cmd, curr->cmds, ep);
		dprt(2, "%s: command not found💀\n", curr->cmds[0]);
		ms()->status = 126;
	}
	free(cmd);
	free_strs(ep);
	free_all(ms()->status, 0);
}
