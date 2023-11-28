/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 20:07:25 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(void)
{
	ep_lclear(&var()->epl);
	free_strs(var()->paths);
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

void	cmd_loop(char **ep)
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
		if (builtin(&status))
			cmd_execute(ep);
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
	if (!path_from_ep)
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

void	create_lstep(char **ep)
{
	int		i;
	char	*cwd;

	i = -1;
	while (ep[++i])
		ep_ladd_back(&var()->epl, ep_lnew(ep[i]));
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	ep_change_value("PWD", cwd);
	free(cwd);
	if (get_env("SHLVL"))
		i = ft_atoi(get_env("SHLVL")->data) + 1;
	else
		i = 1;
	cwd = ft_itoa(i);
	ep_change_value("SHLVL", cwd);
	free(cwd);
}

int	main(int ac, char **av, char **ep)
{
	var()->ac = ac;
	var()->av = av;
	if (!ep)
		return (0);
	create_lstep(ep);
	parsing_paths(ep, -1);
	cmd_loop(ep);
	prt("exit\n");
	free_all();
	return (var()->status);
}

/*
make the env path work as intended
pass envp to execve in char **
fix SHLVL and _. PATH is impossible
gets as input the last </<< from the prompt
Execution "tree" WIP:
redirect from the last redirected file

redirect to the last redirected file

if pipe start all over again
*/
