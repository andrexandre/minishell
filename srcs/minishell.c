/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/12/05 17:49:32 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(int exit_code)
{
	close(var()->saved_fd[0]);
	close(var()->saved_fd[1]);
	close(0);
	close(1);
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
		free(buf);
		execution(&status);
		ft_lstclear(&var()->lst_lexer, free_lst);
		ft_lstclear(&var()->words, free_lst);
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
	close(var()->fd[0]);
	close(var()->fd[1]);
}

void	parsing_paths(void)
{
	char	*temp;
	int		i;

	if (!get_env("PATH"))
		return ;
	free_strs(var()->paths);
	var()->paths = ft_split(get_env("PATH")->data, ':');
	if (!var()->paths)
		return ;
	i = -1;
	while (var()->paths[++i])
	{
		temp = var()->paths[i];
		(var()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!var()->paths[i])
			return ;
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
	else
	{
		str = ft_strjoin("/.local/bin:", get_env("PATH")->data);
		ep_change_value("PATH", str);
		free(str);
	}
	ep_lnew_add_back(&var()->epl, cwd);
	free(cwd);
	var()->saved_fd[0] = dup(STDIN_FILENO);
	var()->saved_fd[1] = dup(STDOUT_FILENO);
}

void	minishell_init(char **ep)
{
	int		i;
	char	*cwd;
	char	*str;

	i = -1;
	while (ep && ep[++i])
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

// this is a temporary function for debugging that uses unauthorized functions
void	debug(int n)
{
	char	*hist = "/nfs/homes/analexan/minishell/.minishell_history";
	char	*asd = NULL;
	char	history_file[100];

	if (!get_env("HOME"))
		strcpy(history_file, hist);
	else
	{
		asd = ft_strjoin(get_env("HOME")->data, "/minishell/.minishell_history");
		strcpy(history_file, asd);
		free(asd);
	}
	if (!n)
		read_history(history_file);
	else
		write_history(history_file);
}
#include <limits.h>
int	main(int ac, char **av, char **ep)
{
	var()->ac = ac;
	var()->av = av;
	minishell_init(ep);
	parsing_paths();
	debug(0);
	cmd_loop();
	prt("exit\n");
	debug(1);
	rl_clear_history();
	free_all(var()->status);
}

/*
o expander pode aumentar / diminuir a lst
e o heredoc é a execão
*/