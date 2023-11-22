/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/22 21:59:11 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(void)
{
	ep_lclear(&var()->epl);
	free_strs(var()->paths);
}

void	cmd_loop(char **ep)
{
	char	**cmdargs;
	char	*buf;
	int		status;

	status = 1;
	while (status)
	{
		buf = readline("\033[0;34mminishell\033[0m😎> ");
		if (!buf)
			break ;
		add_history(buf);
		var()->lst_parse = NULL;
		var()->words = NULL;
		lexer(buf);
		parse();
		cmdargs = ft_split(buf, ' ');
		int i = -1;
		while (cmdargs[++i])
			ft_lstadd_back(&var()->words, ft_lstnew(ft_strdup(cmdargs[i])));
		if (builtin(&status))
			cmd_execute(ep);
		ft_lstclear(&var()->words, free);
		ft_lstclear(&var()->lst_parse, free_word);
		free_strs(cmdargs);
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
	char	*str;

	i = -1;
	cwd = NULL;
	while (ep[++i])
		ep_ladd_back(&var()->epl, ep_lnew(ft_strdup(ep[i])));
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	str = ft_strjoin("PWD=", cwd);
	free(cwd);
	ep_export_value(str);
	free(str);
}

void	handler(int num)
{
	if (num == SIGINT)
	{
		prt("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int ac, char **av, char **ep)
{
	var()->ac = ac;
	var()->av = av;
	if (!ep)
		return (0);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	create_lstep(ep);
	parsing_paths(ep, -1);
	cmd_loop(ep);
	prt("exit\n");
	free_all();
	return (0);
}

/*
gets as input the last </<< from the prompt
redirect from the last redirected file

redirect to the last redirected file

if pipe start all over again
*/