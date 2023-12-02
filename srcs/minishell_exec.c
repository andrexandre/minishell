/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/12/02 19:11:38 by analexan         ###   ########.fr       */
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

// receive input from the stdin and save in in the file
int	get_stdin(char *arg)
{
	char	*buf;
	char	*str;
	int		fd;

	fd = open("/tmp/msh-hd", O_WRONLY | O_CREAT | O_TRUNC , 0600);
	if (fd < 0)
		return (-1);
	prt("> ");
	buf = get_next_line(STDIN_FILENO);
	str = ft_strjoin(arg, "\n");
	while (ft_strcmp(buf, str))
	{
		write(fd, buf, ft_strlen(buf));
		free(buf);
		prt("> ");
		buf = get_next_line(STDIN_FILENO);
	}
	free(str);
	free(buf);
	close(fd);
	fd = open("/tmp/msh-hd", O_RDONLY);
	if (fd < 0)
		return (-1);
	unlink("/tmp/msh-hd");
	return (fd);
}

// void	process_hd(char **cmdargs, char **av, char **ep, int mode)
// {
// 	char	*cmd;
// 	int		fd;
// 	int		fd2;
// 	fd = var()->pipe[mode][0];
// 	if (mode)
// 		fd2 = open(av[var()->ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
// 	else
// 		fd2 = var()->pipe[1][1];
// 	if (mode && fd2 < 0)
// 		error_b(3);
// 	cmd = search_cmd(cmdargs, fd, fd2);
// 	if (dup2(fd, STDIN_FILENO) < 0 || dup2(fd2, STDOUT_FILENO) < 0)
// 		error_b(2);
// 	close_all(fd, fd2);
// 	execve(cmd, cmdargs, ep);
// 	free(cmd);
// 	perror(cmdargs[0]);
// 	free_all(0);
// 	exit(127);
// }

int	redirections(void)
{
	char	**cmds;
	t_list	*curr;
	int		i;

	curr = var()->words;
	while (curr)
	{
		cmds = var()->words->cmds;
		i = -1;
		while (curr->cmds[++i])
		{
			if ((!ft_strcmp(curr->cmds[i], "<") || !ft_strcmp(curr->cmds[i], "<<")
				|| !ft_strcmp(curr->cmds[i], ">") || !ft_strcmp(curr->cmds[i], ">>")) && !curr->cmds[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				var()->status = 2;
				return (1);
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
					perror(curr->cmds[i + 1]);
					var()->fd[0] = 0;
					return (1);
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(var()->fd[0]);
					var()->fd[0] = 0;
					break ;
				}
				var()->words->cmds = remove_items(cmds, i);
				cmds = var()->words->cmds;
				i--;
			}
			else if (curr->cmds[i][0] == '>')
			{
				if (var()->fd[1] < 0)
				{
					perror(curr->cmds[i + 1]);
					var()->fd[1] = 0;
					return (1);
				}
				if (ft_strlen_matrix(curr->cmds) < 3)
				{
					close(var()->fd[1]);
					var()->fd[1] = 0;
					break ;
				}
				var()->words->cmds = remove_items(cmds, i);
				cmds = var()->words->cmds;
				i--;
			}
		}
		curr = curr->next;
	}
	return (0);
}

void	execution(int *status)
{
	char	**ep;
	t_list	*curr;
	int		i;
	int		j;
	int		needs_dup[2];

	/*
	to-do:
	fazer pipes sozinhos (sem redirecionamento)
	fazer pipe e depois uma child
	(bultin ou nao)(prob will fix heredoc w ctl+c/d)
	*/
	needs_dup[0] = 0;
	needs_dup[1] = 0;
	curr = var()->words;
	// if (redirections())
	// 	return ;
	j = 0;
	// int is_piped = 0;
	while (curr)
	{
		i = -1;
		while (curr->cmds[++i])
		{
			if ((!ft_strcmp(curr->cmds[i], "<") || !ft_strcmp(curr->cmds[i], "<<")
				|| !ft_strcmp(curr->cmds[i], ">") || !ft_strcmp(curr->cmds[i], ">>")) && !curr->cmds[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
				var()->status = 2;
				break ;
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
		// if (curr->next)
		// {
		// 	if (pipe(var()->pipe) < 0)
		// 		perror("pipe");
		// 	is_piped = 1;
		// }
		// if (is_piped)
		// {
		// 	close(var()->pipe[j]);
		// 	if (dup2(var()->pipe[!j], !j) < 0)
		// 		perror("dup2, pipe");
		// 	close(var()->pipe[!j]);
		// }
		if (needs_dup[0])
		{
			if (dup2(var()->fd[0], STDIN_FILENO) < 0)
				perror("dup2, fd[0]");
			needs_dup[0] = 0;
		}
		if (needs_dup[1])
		{
			if (dup2(var()->fd[1], STDOUT_FILENO) < 0)
				perror("dup2, fd[1]");
			needs_dup[1] = 0;
		}
		if (run_builtin())
			return ;
		else if (!ft_strcmp(curr->cmds[0], "exit")
			|| !ft_strcmp(curr->cmds[0], "q"))
			*status = 0;
		else
		{
			ep = ep_from_epl();
			parsing_paths();
			cmd_execute(ep);
			free_strs(ep);
		}
		curr = curr->next;
		j++;
	}
}

/*
ls | cat < zxc
nao executa nada pcausa que n existe zxc

cat > out | cat > outt
so o primeiro comando

cat < out | cat < outt
so o segundo comando

ls > out | ls > outt
faz os dois
*/
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
