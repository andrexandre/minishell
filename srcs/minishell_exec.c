/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/12/09 15:45:45 by analexan         ###   ########.fr       */
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
		var()->status = (*prt_eplst)();
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
// fix hd ctrl+c
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
		prt("'exit'\n");
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
	var()->status = 128 + sig;
}

void	free_pipes(void)
{
	int	i;
	int	len;

	i = -1;
	len = ft_lstsize(var()->words);
	while (++i < len - 1)
		free(var()->pipe[i]);
	if (len > 1)
		free(var()->pipe);
	free(var()->pid);
}

/*
cat < out > asd | cat < zxc
executa o pipe em que existe tudo (zxc n existe)

cat > out | cat > outt
so o primeiro comando

cat < out | cat < outt
so o segundo comando

ls > out | ls > outt
faz os dois
*/
int		needs_dup[2];
void	execution(int *status)
{
	t_list	*curr;
	int		i;
	int		j;
	int		len;

	/*
	to-do:
	fazer pipe e depois uma child
	(bultin ou nao)(prob will fix heredoc w ctl+c/d)
	if theres pipe, builtins are run in child
	*/
	curr = var()->words;
	len = ft_lstsize(var()->words);
	// if (len > 1)
	// 	var()->pipe = ft_calloc(len - 1, sizeof(int *));
	// j = 0;
	// while (j < len - 1)
	// {
	// 	var()->pipe[j] = ft_calloc(2, sizeof(int));
	// 	if (pipe(var()->pipe[j]) < 0)
	// 		perror("pipe");
	// 	j++;
	// }
	var()->pid = ft_calloc(len, sizeof(int));
	j = 0;
	while (curr)
	{
		i = -1;
		// var()->fd[0] = 0;
		// var()->fd[1] = 0;
		// if (j)
		// 	var()->fd[0] = var()->pipe[j - 1][0];
		// if (j != len - 1)
		// 	var()->fd[1] = var()->pipe[j][1];
		needs_dup[0] = 0;
		needs_dup[1] = 0;
		while (curr->cmds[++i])
		{
			if ((!ft_strcmp(curr->cmds[i], "<") || !ft_strcmp(curr->cmds[i], "<<")
				|| !ft_strcmp(curr->cmds[i], ">") || !ft_strcmp(curr->cmds[i], ">>")) && !curr->cmds[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				var()->status = 2;
				return ;
			}
			if (var()->fd[0] && curr->cmds[i][0] == '<')
				close(var()->fd[0]);
			if (var()->fd[1] && curr->cmds[i][0] == '>')
				close(var()->fd[1]);
			if (!ft_strcmp(curr->cmds[i], "<"))
				var()->fd[0] = open(curr->cmds[i + 1], O_RDONLY);
			else if (!ft_strcmp(curr->cmds[i], "<<"))
				var()->fd[0] = get_stdin(curr->cmds[i + 1]);
			else if (!ft_strcmp(curr->cmds[i], ">"))
				var()->fd[1] = open(curr->cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (!ft_strcmp(curr->cmds[i], ">>"))
				var()->fd[1] = open(curr->cmds[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				continue ;
			if (curr->cmds[i][0] == '<')
			{
				if (var()->fd[0] < 0)
				{
					if (!g_sig && var()->fd[0] != -2)
						perror(curr->cmds[i + 1]);
					var()->fd[0] = 0;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) > 2)
					needs_dup[0] = 1;
				else
				{
					close(var()->fd[0]);
					var()->fd[0] = 0;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
			else if (curr->cmds[i][0] == '>')
			{
				if (var()->fd[1] < 0)
				{
					if (!g_sig)
						perror(curr->cmds[i + 1]);
					var()->fd[1] = 0;
					break ;
				}
				if (ft_strlen_matrix(curr->cmds) > 2)
					needs_dup[1] = 1;
				else
				{
					close(var()->fd[1]);
					var()->fd[1] = 0;
					break ;
				}
				curr->cmds = remove_items(curr->cmds, i);
				i--;
			}
		}
		if (needs_dup[0] && !var()->words->next)
		{
			if (dup2(var()->fd[0], STDIN_FILENO) < 0)
				perror("dup2, fd[0]");
			needs_dup[0] = 0;
		}
		if (needs_dup[1] && !var()->words->next)
		{
			if (dup2(var()->fd[1], STDOUT_FILENO) < 0)
				perror("dup2, fd[1]");
			needs_dup[1] = 0;
		}
		if (!var()->words->next && run_builtin())
			(void)var;
		else if (!ft_strcmp(curr->cmds[0], "exit")
			|| !ft_strcmp(curr->cmds[0], "q"))
			*status = 0;
		else
		{
			char *cmd = search_cmd(curr->cmds[0]);
			if (cmd)
			{
				signal(SIGINT, tmp_handler);
				signal(SIGQUIT, tmp_handler);
				var()->pid[j] = fork();
				if (var()->pid[j] < 0)
					perror("fork");
				if (!var()->pid[j])
					cmd_execute(cmd, ep_from_epl(), curr);
				free(cmd);
			}
		}
		if (var()->fd[0])
			close(var()->fd[0]);
		if (var()->fd[1])
			close(var()->fd[1]);
		if (var()->fd[0] && !var()->words->next)
		{
			if (dup2(var()->saved_fd[0], STDIN_FILENO) < 0)
				perror("dup2, saved_fd[0]");
			var()->fd[0] = 0;
		}
		if (var()->fd[1] && !var()->words->next)
		{
			if (dup2(var()->saved_fd[1], STDOUT_FILENO) < 0)
				perror("dup2, saved_fd[1]");
			var()->fd[1] = 0;
		}
		// if (j)
		// 	close(var()->pipe[j - 1][0]);
		// if (j != len - 1)
		// 	close(var()->pipe[j][1]);
		curr = curr->next;
		j++;
	}
	int stat = 0;
	j = -1;
	while (++j < len)
	{
		waitpid(var()->pid[j], &stat, 0);
		if (WIFEXITED(stat))
			var()->status = WEXITSTATUS(stat);
		else
			var()->status = 130;
	}
	// free_pipes();
	free(var()->pid);
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
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
	var()->status = 127;
	return (NULL);
}

void	cmd_execute(char *cmd, char **ep, t_list *curr)
{
	// int	i;

	parsing_paths();
	if (needs_dup[0] && var()->fd[0] && dup2(var()->fd[0], STDIN_FILENO) < 0)
		perror("dup2, fd[0]");
	if (needs_dup[1] && var()->fd[1] && dup2(var()->fd[1], STDOUT_FILENO) < 0)
		perror("dup2, fd[1]");
	// i = -1;
	// while (++i < ft_lstsize(var()->words) - 1)
	// {
	// 	close(var()->pipe[i][0]);
	// 	close(var()->pipe[i][1]);
	// }
	execve(cmd, curr->cmds, ep);
	ft_putstr_fd(curr->cmds[0], 2);
	ft_putstr_fd(": command not found💀\n", 2);
	free(cmd);
	// free_pipes();
	free(var()->pid);
	ft_lstclear(&var()->words, free_lst);
	ft_lstclear(&var()->lst_lexer, free_lst);
	free_strs(ep);
	free_all(126);
}
