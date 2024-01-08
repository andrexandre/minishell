/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 17:19:13 by analexan          #+#    #+#             */
/*   Updated: 2024/01/08 17:40:31 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (ft_strcmp(buf, "q") && *buf)
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
	ep_lnew(&ms()->epl, cwd);
	free(cwd);
	ms()->saved_fd[0] = dup(STDIN_FILENO);
	if (ms()->saved_fd[0] < 0)
		free_all(EXIT_FAILURE, "dup");
	ms()->saved_fd[1] = dup(STDOUT_FILENO);
	if (ms()->saved_fd[1] < 0)
		free_all(EXIT_FAILURE, "dup");
}

void	minishell_init(char **ep)
{
	int		i;
	char	*cwd;
	char	*str;

	i = -1;
	while (ep && ep[++i])
		if (ft_strncmp(ep[i], "_=", 2))
			ep_lnew(&ms()->epl, ep[i]);
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
