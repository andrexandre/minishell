/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/29 19:12:06 by analexan         ###   ########.fr       */
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
	while (status)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		buf = readline("\033[0;34mminishell\033[0m😎> ");
		if (!buf)
			break ;
		if (*buf)
			add_history(buf);
		lexer(buf);
		parse();
		execution(&status);
		ft_lstclear(&var()->lst_lexer, free_lst);
		ft_lstclear(&var()->words, free_lst);
		free(buf);
	}
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

	if (get_env("SHLVL"))
		num = ft_atoi(get_env("SHLVL")->data) + 1;
	else
		num = 1;
	str = ft_itoa(num);
	ep_change_value("SHLVL", str);
	free(str);
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
	var()->ac = ac;
	var()->av = av;
	if (!ep)
		return (0);
	minishell_init(ep);
	parsing_paths(ep, -1);
	cmd_loop();
	prt("exit\n");
	free_all(var()->status);
}

/*
gets as input the last </<< from the prompt
Execution "tree" WIP:
redirect from the last redirected file

redirect to the last redirected file

if pipe start all over again
o expander pode aumentar / diminuir a lst
e o heredoc é a execão
*/
