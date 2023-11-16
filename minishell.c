/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/16 13:53:59 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(void)
{
	t_list	*current;

	while (var()->ep)
	{
		current = var()->ep->next;
		free(var()->ep->name);
		ft_lstdelone(var()->ep, free);
		var()->ep = current;
	}
	free_strs(var()->paths);
	// t_word *ptr = var()->lstep_parsed->content;
	// while (var()->lstep_parsed)
	// {
	// 	ptr = var()->lstep_parsed->content;
	// 	current = var()->lstep_parsed->next;
	// 	free(ptr->str);
	// 	ft_lstdelone(var()->lstep_parsed, free);
	// 	var()->lstep_parsed = current;
	// }
	// ptr = var()->words->content;
	// while (var()->words)
	// {
	// 	ptr = var()->words->content;
	// 	current = var()->words->next;
	// 	free(ptr->str);
	// 	free(ptr);
	// 	free(var()->words);
	// 	var()->words = current;
	// }
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
		cmdargs = ft_split(buf, ' ');
		var()->lstep_parsed = NULL;
		var()->words = NULL;
		add_history(buf);
		// lexer(buf);
		// parse();
		if (builtin(cmdargs, &status))
			cmd_execute(cmdargs, ep);
		free(buf);
		free_strs(cmdargs);
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
	t_list	*new;

	i = -1;
	cwd = NULL;
	while (ep[++i])
	{
		new = ft_lstnew(ft_strdup(ep[i]));
		ft_lstadd_back(&var()->ep, new);
		new->name = ft_substr(new->content, 0, ft_strlen(new->content)
				- ft_strlen(ft_strchr(new->content, '=')));
		new->data = ft_strchr(new->content, '=') + 1;
	}
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	str = ft_strjoin("PWD=", cwd);
	run_export((char *[]){"export", str, NULL});
	free(str);
	free(cwd);
}

// fix some bugs with SIGINT
void	handler(int num)
{
	(void)num;
	prt("\n\033[0;34mminishell\033[0m😎> ");
}

int	main(int ac, char **av, char **ep)
{
	var()->ac = ac;
	var()->av = av;
	if (!ep)
		return (0);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	create_lstep(ep);
	parsing_paths(ep, -1);
	cmd_loop(ep);
	prt("exit\n");
	free_all();
	return (0);
}
