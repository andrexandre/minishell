/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2024/01/09 15:20:58 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if len < 0 close (length of words) pipes
void	close_pipes(int len)
{
	int	i;

	i = -1;
	if (len < 0)
		len = ft_lstsize(ms()->words) - 1;
	while (ms()->pipe && ++i < len)
	{
		close(ms()->pipe[i][0]);
		close(ms()->pipe[i][1]);
	}
}

void	free_pipes_words(void)
{
	int	i;
	int	len;

	i = -1;
	len = ft_lstsize(ms()->words);
	while (ms()->pipe && ++i < len - 1)
		free(ms()->pipe[i]);
	if (len > 1)
		free(ms()->pipe);
	free(ms()->pid);
	ms()->pid = NULL;
	ft_lstclear(&ms()->words);
	ft_lstclear(&ms()->lst_lexer);
}

void	free_all(int exit_code, char *err_msg)
{
	close(ms()->fd[0]);
	close(ms()->fd[1]);
	close(ms()->saved_fd[0]);
	close(ms()->saved_fd[1]);
	close(0);
	close(1);
	close(2);
	ep_lclear(&ms()->epl);
	free_strs(ms()->paths);
	free_pipes_words();
	if (exit_code == 1)
		perror(err_msg);
	exit(exit_code);
}

void	prt_strs(char **strs, int n)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		prt("%s", strs[i]);
		if (strs[i + 1])
			prt(" ");
	}
	if (n)
		prt("\n");
}

t_var	*ms(void)
{
	static t_var	ms;

	return (&ms);
}
