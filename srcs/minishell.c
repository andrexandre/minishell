/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2024/01/11 21:31:36 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NEEDS TO DECREASE LINES
void	prompt_loop(void)
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
		if (*buf)
			add_history(buf);
		else
		{
			free(buf);
			continue ;
		}
		if (lexer(buf))
		{
			free(buf);
			continue ;
		}
		if (parse())
			execution();
		free(buf);
		free_pipes_words();
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

// this is a temporary function for debugging that uses unauthorized functions
void	debug(int n)
{
	char	*hist;

	hist = ".minishell_history";
	if (!n)
		read_history(hist);
	else
		write_history(hist);
}

/* antes de entregar, retirar:
norm do prompt loop
a funcão debug

302 -> 317 certas no mpanic
*/
int	main(int ac, char **av, char **ep)
{
	ms()->debug = 0;
	(void)ac;
	(void)av;
	minishell_init(ep);
	debug(0);
	prompt_loop();
	debug(1);
	rl_clear_history();
	free_all(ms()->status, 0);
}
