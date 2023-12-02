/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 10:55:20 by jealves-          #+#    #+#             */
/*   Updated: 2023/12/02 22:50:05 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansion(char *str)
{
	t_eplist	*curr;

	curr = var()->epl;
	while (curr)
	{
		if (ft_strcmpold(curr->name, str))
		{
			prt("\033[1;34m");
			prt("Expander: busca = $%s, resultado = %s\n", str, curr->data);
			prt("\033[0m");
			free(str);
			return (ft_strdup(curr->data));
		}
		curr = curr->next;
	}
	free(str);
	return (ft_strdup(""));
}

char	*expander(char *str)
{
	int		i;
	int		j;
	char	*ex_str;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$' || str[i] == '~')
		{
			j = i;
			while (str[++j])
				if (!ft_isalnum(str[j]))
					break ;
			ex_str = expansion(ft_substr(str, i + 1, j - i - 1));
			ft_strrep(&str, i, j, ex_str);
			free(ex_str);
			i = 0;
		}
	}
	return (str);
}
