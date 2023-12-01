/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/12/01 18:28:38 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(int exit_code)
{
	ep_lclear(&var()->epl);
	free_strs(var()->paths);
	exit(exit_code);
}

void	handler(int num)
{
	if (num == SIGQUIT)
		prt("Quit (core dumped)\n");
	if (num == SIGINT)
	{
		prt("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	cmd_loop(void)
{
	char	*buf;
	int		status;

	status = 1;
	var()->saved_fd[0] = dup(STDIN_FILENO);
	var()->saved_fd[1] = dup(STDOUT_FILENO);
	while (status)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		buf = readline("\033[0;34mminishell\033[0m😎> ");
		if (!buf)
			break ;
		if (*buf && ft_strcmp(buf, "q"))
			add_history(buf);
		else if (ft_strcmp(buf, "q"))
		{
			free(buf);
			continue ;
		}
		lexer(buf);
		parse();
		execution(&status);
		ft_lstclear(&var()->lst_lexer, free_lst);
		ft_lstclear(&var()->words, free_lst);
		free(buf);
		if (var()->fd[0])
		{
			close(var()->fd[0]);
			dup2(var()->saved_fd[0], STDIN_FILENO);
		}
		if (var()->fd[1])
		{
			close(var()->fd[1]);
			dup2(var()->saved_fd[1], STDOUT_FILENO);
		}
	}
	close(var()->saved_fd[0]);
	close(var()->saved_fd[1]);
	close(0);
	close(1);
}

void	parsing_paths(char **ep, int i)
{
	char	*path_from_ep;
	char	*temp;

	path_from_ep = NULL;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=", 5);
		if (path_from_ep)
			break ;
	}
	if (!path_from_ep || !ep[i])
		return ;
	path_from_ep += 5;
	var()->paths = ft_split(path_from_ep, ':');
	if (!var()->paths)
		exit(EXIT_FAILURE);
	i = -1;
	while (var()->paths[++i])
	{
		temp = var()->paths[i];
		(var()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!var()->paths[i])
			exit(EXIT_FAILURE);
	}
}

void	var_init(char *cwd)
{
	int		num;
	char	*str;

	num = ft_atoi(getenv("SHLVL")) + 1;
	str = ft_itoa(num);
	ep_change_value("SHLVL", str);
	free(str);
	if (!get_env("PATH"))
		ep_change_value("PATH", "/.local/bin:/usr/local/sbin:\
/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	ep_lnew_add_back(&var()->epl, cwd);
	free(cwd);
}

void	minishell_init(char **ep)
{
	int		i;
	char	*cwd;
	char	*str;

	i = -1;
	while (ep[++i])
	{
		if (!ft_strcmp(ep[i], "_="))
			continue ;
		ep_lnew_add_back(&var()->epl, ep[i]);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	ep_change_value("PWD", cwd);
	str = ft_strjoin(cwd, "/minishell");
	free(cwd);
	cwd = ft_strjoin("_=", str);
	free(str);
	var_init(cwd);
}

int	main(int ac, char **av, char **ep)
{
	// int fd;
	// fd = open("asd", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// close(fd);
	// fd = open("out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// if (dup2(fd, STDOUT_FILENO) < 0)
	// 	perror("dup2");
	// close(fd);
	// return (0);
	var()->ac = ac;
	var()->av = av;
	if (!ep)
		return (0);
	minishell_init(ep);
	parsing_paths(ep, -1);
	char *asd = ft_strjoin(get_env("HOME")->data, "/minishell/.minishell_history");
	char history_file[100];
	strcpy(history_file, asd);
	free(asd);
	read_history(history_file); // illegal
	cmd_loop();
	prt("exit\n");
	write_history(history_file); // illegal
	rl_clear_history();
	free_all(var()->status);
}

/*
cat < out < asd: fix
if i use pipe, the next command, builtin or not will make a child

o expander pode aumentar / diminuir a lst
e o heredoc é a execão
*/