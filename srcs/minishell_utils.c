/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/23 22:28:54 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*var(void)
{
	static t_var	var;

	return (&var);
}

void	prt_strs(char **strs, char sep)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		prt("%s", strs[i]);
		if (strs[i + 1])
			prt("%c", sep);
	}
}

void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return (NULL);
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (NULL);
}

void	print_lst(t_list *lst, int n)
{
	while (lst)
	{
		if (!lst->next && n)
			prt("%s", lst->str);
		else
			prt("%s\n", lst->str);
		lst = lst->next;
	}
}

void	free_lst(t_list *word)
{
	free_strs(word->cmds);
	free(word->str);
}
