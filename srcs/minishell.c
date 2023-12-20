/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/12/20 19:08:18 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig = 0;

void	free_all(int exit_code)
{
	close(ms()->saved_fd[0]);
	close(ms()->saved_fd[1]);
	close(0);
	close(1);
	close(2);
	ep_lclear(&ms()->epl);
	free_strs(ms()->paths);
	exit(exit_code);
}

void	handler(int sig)
{
	g_sig = sig;
	if (sig == SIGINT)
	{
		prt("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	ms()->status = 128 + sig;
}

void	cmd_loop(void)
{
	char	*buf;

	ms()->running = 1;
	while (ms()->running)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		buf = readline("\033[0;34mminishell\033[0m😎> ");
		if (!buf)
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 6);
			break ;
		}
		if (ft_strcmp(buf, "q"))
			add_history(buf);
		else if (!*buf)
			continue ;
		if (lexer(buf))
		{
			free(buf);
			continue;
		}
		parse();
		free(buf);
		execution();
		ft_lstclear(&ms()->lst_lexer, free_lst);
		ft_lstclear(&ms()->words, free_lst);
	}
	close(ms()->fd[0]);
	close(ms()->fd[1]);
}

void	parsing_paths(void)
{
	char	*temp;
	int		i;

	if (!get_env("PATH"))
		return ;
	free_strs(ms()->paths);
	ms()->paths = ft_split(get_env("PATH")->data, ':');
	if (!ms()->paths)
		return ;
	i = -1;
	while (ms()->paths[++i])
	{
		temp = ms()->paths[i];
		(ms()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!ms()->paths[i])
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
// 	if (!get_env("PATH"))
// 		ep_change_value("PATH", "/.local/bin:/usr/local/sbin:///
// /usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
// 	else
// 	{
// 		str = ft_strjoin("/.local/bin:", get_env("PATH")->data);
// 		ep_change_value("PATH", str);
// 		free(str);
// 	}
	ep_lnew_add_back(&ms()->epl, cwd);
	free(cwd);
	ms()->saved_fd[0] = dup(STDIN_FILENO);
	ms()->saved_fd[1] = dup(STDOUT_FILENO);
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
		ep_lnew_add_back(&ms()->epl, ep[i]);
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
	ms()->debug = 0;
	ms()->ac = ac;
	ms()->av = av;
	minishell_init(ep);
	parsing_paths();
	debug(0);
	cmd_loop();
	debug(1);
	rl_clear_history();
	free_all(ms()->status);
}
// progress i was in status tests
/*
o expander pode aumentar / diminuir a lst
e o heredoc é a execão
*/