/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/27 17:29:14 by analexan         ###   ########.fr       */
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
	char	*str;

	while (lst)
	{
		str = lst->str;
		if (ft_strchr(str, '$'))
		{
			str = ft_strchr(str, '$');
			str++;
			if (!ft_strcmp(str, "?"))
				prt("%d", var()->status);
			else if (get_env(str))
				prt("%s", get_env(str)->data);
			if (lst->next)
				prt(" ");
			else
				prt("\n");
		}
		else if (!lst->next && n)
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
