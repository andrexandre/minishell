/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 10:55:20 by jealves-          #+#    #+#             */
/*   Updated: 2023/12/05 23:46:33 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansion(char *str)
{
	t_eplist	*curr;
	char		*res;

	curr = get_env(str);
	if (str[0] == '$')
		res = ft_itoa(getpid());
	else if (str[0] == '?')
		res = ft_itoa(var()->status);
	else if (str[0] == '"' || str[0] == '\'')
		res = ft_strdup(str);
	else if (curr)
		res = ft_strdup(curr->data);
	else
		res = ft_strdup("");
	prt("\033[1;34m");
	prt("Expander: busca = $%s, resultado = %s\n", str, res);
	prt("\033[0m");
	free(str);
	return (res);
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
				if (!ft_isalnum(str[j]) && str[j] != '_')
					break ;
			if (j == i + 1)
				j++;
			ex_str = expansion(ft_substr(str, i + 1, j - i - 1));
			ft_strrep(&str, i, j, ex_str);
			free(ex_str);
			i = 0;
		}
	}
	return (str);
}
