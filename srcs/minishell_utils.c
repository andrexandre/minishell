/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 18:06:09 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*var(void)
{
	static t_var	var;

	return (&var);
}

void	prt_strs(char **strs, int n)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		if (ft_strchr(strs[i], '$'))
		{
			strs[i] = ft_strchr(strs[i], '$');
			if (!ft_strcmp((strs[i] + 1), "?"))
				prt("%d", var()->status);
			else if (get_env(strs[i] + 1))
				prt("%s", get_env(strs[i] + 1)->data);
		}
		else
			prt("%s", strs[i]);
		if (strs[i + 1])
			prt(" ");
	}
	if (!n)
		prt("\n");
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

void	free_lst(t_list *word)
{
	free_strs(word->cmds);
	free(word->str);
}
