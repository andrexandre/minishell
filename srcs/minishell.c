/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2024/01/08 17:39:13 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// this is a temporary function for debugging that uses unauthorized functions
void	debug(int n)
{
	char	*hist = "/home/analexan/minishell/.minishell_history";
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
	(void)av;
	minishell_init(ep);
	parsing_paths(); // ver se isto é preciso
	debug(0);
	cmd_loop();
	debug(1);
	rl_clear_history();
	free_all(ms()->status, 0);
}
