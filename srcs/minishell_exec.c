/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/12/20 19:09:26 by analexan         ###   ########.fr       */
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

void	tmp_handler(int sig);
// receive input from the stdin and save in in the file
int	get_stdin(char *arg)
{
	char	*buf;
	int		fd;

	signal(SIGINT, tmp_handler);
	fd = open("/tmp/msh-hd", O_WRONLY | O_CREAT | O_TRUNC , 0600);
	if (fd < 0)
		return (-1);
	g_sig = 0;
	buf = readline("> ");
	while (!g_sig && buf && ft_strcmp(buf, arg))
	{
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
		buf = readline("> ");
	}
	close(fd);
	signal(SIGINT, handler);
	if (!buf)
	{
		prt("end-of-file (wanted `%s')\n", arg);
		return (-2);
	}
	free(buf);
	if (g_sig)
		return (-1);
	fd = open("/tmp/msh-hd", O_RDONLY);
	if (fd < 0)
		return (-1);
	unlink("/tmp/msh-hd");
	return (fd);
}

char	*search_cmd(char *command);

void	tmp_handler(int sig)
{
	g_sig = sig;
	if (sig == SIGQUIT)
		prt("Quit (core dumped)\n");
	if (sig == SIGINT)
		prt("\n");
	ms()->status = 128 + sig;
}

void	free_pipes(void)
{
	int	i;
	int	len;

	i = -1;
	len = ft_lstsize(ms()->words);
	while (++i < len - 1)
		free(ms()->pipe[i]);
	if (len > 1)
		free(ms()->pipe);
	free(ms()->pid);
}

void	execution(void)
{
	t_list	*curr;
	int		i;
	int		j;
	int		len;
	int		needs_exec;

	curr = ms()->words;
	len = ft_lstsize(ms()->words);
	if (len > 1)
		ms()->pipe = ft_calloc(len - 1, sizeof(int *));
	j = 0;
	while (j < len - 1)
	{
		ms()->pipe[j] = ft_calloc(2, sizeof(int));
		if (pipe(ms()->pipe[j]) < 0)
			perror("pipe");
		j++;
	}
	ms()->pid = ft_calloc(len, sizeof(int));
	j = 0;
	while (curr)
	{
		needs_exec = 1;
		i = -1;
		ms()->fd[0] = 0;
		ms()->fd[1] = 0;
		if (j)
			ms()->fd[0] = ms()->pipe[j - 1][0];
		if (j != len - 1)
			ms()->fd[1] = ms()->pipe[j][1];
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
				ms()->fd[0] = get_stdin(curr->cmds[i + 1]);
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
					if (!g_sig && ms()->fd[0] != -2)
						perror(curr->cmds[i + 1]);
					ms()->fd[0] = 0;
					needs_exec = 0;
					ms()->status = 1;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(ms()->fd[0]);
					ms()->fd[0] = 0;
					needs_exec = 0;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
			else if (curr->cmds[i][0] == '>')
			{
				if (ms()->fd[1] < 0)
				{
					if (!g_sig)
						perror(curr->cmds[i + 1]);
					ms()->fd[1] = 0;
					needs_exec = 0;
					ms()->status = 1;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(ms()->fd[1]);
					ms()->fd[1] = 0;
					needs_exec = 0;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
		}
		if (!ms()->words->next && curr->type == BUILT_IN)
		{
			if (ms()->fd[0] && dup2(ms()->fd[0], STDIN_FILENO) < 0)
				perror("dup2, fd[0]");
			if (ms()->fd[1] && dup2(ms()->fd[1], STDOUT_FILENO) < 0)
				perror("dup2, fd[1]");
		}
		if (!needs_exec || (!ms()->words->next && run_builtin(curr)))
			(void)ms;
		else
		{
			char *cmd;
			if (curr->type == BUILT_IN)
				cmd = NULL;
			else
				cmd = search_cmd(curr->cmds[0]);
			if (cmd || curr->type == BUILT_IN)
			{
				signal(SIGINT, tmp_handler);
				signal(SIGQUIT, tmp_handler);
				ms()->pid[j] = fork();
				if (ms()->pid[j] < 0)
					perror("fork");
				if (!ms()->pid[j])
					cmd_execute(cmd, ep_from_epl(), curr);
				free(cmd);
			}
			else
				needs_exec = 0;
		}
		if (ms()->fd[0] && !close(ms()->fd[0]))
			if (!ms()->words->next && dup2(ms()->saved_fd[0], STDIN_FILENO) < 0)
				perror("dup2, saved_fd[0]");
		if (ms()->fd[1] && !close(ms()->fd[1]))
			if (!ms()->words->next && dup2(ms()->saved_fd[1], STDOUT_FILENO) < 0)
				perror("dup2, saved_fd[1]");
		curr = curr->next;
		j++;
	}
	int stat = 0;
	j = -1;
	if (!needs_exec && ms()->words->next)
		needs_exec = 1;
	// needs_exec is here so when a command didnt execute we dont wait for it
	while (needs_exec && !(ms()->words->type == BUILT_IN && !ms()->words->next) && ++j < len)
	{
		waitpid(ms()->pid[j], &stat, 0);
		if (WIFEXITED(stat))
			ms()->status = WEXITSTATUS(stat);
	}
	free_pipes();
}

char	*search_cmd(char *command)
{
	int			i;
	char		*cmd;
	struct stat	statbuf;

	i = -1;
	if (ft_strchr(command, '/') && (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)))
	{
		dprt(2, "%s: is a directory\n", command);
		ms()->status = 126;
		// not working exit status
		return (NULL);
	}
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
		if (!access(command, F_OK | X_OK))
			return (ft_strdup(command));
	dprt(2, "%s: command not found\n", command);
	ms()->status = 127;
	return (NULL);
}

void	cmd_execute(char *cmd, char **ep, t_list *curr)
{
	int	i;

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
	if (run_builtin(curr))
	{
		free(cmd);
		free_pipes();
		ft_lstclear(&ms()->words, free_lst);
		ft_lstclear(&ms()->lst_lexer, free_lst);
		free_strs(ep);
		exit(ms()->status);
	}
	execve(cmd, curr->cmds, ep);
	ft_putstr_fd(curr->cmds[0], 2);
	ft_putstr_fd(": command not found💀\n", 2);
	free(cmd);
	free_pipes();
	ft_lstclear(&ms()->words, free_lst);
	ft_lstclear(&ms()->lst_lexer, free_lst);
	free_strs(ep);
	free_all(126);
}
