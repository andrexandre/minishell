/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 10:55:20 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/08 21:01:16 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansion(char *str)
{
	char	*res;

	if (str[0] == '$')
		res = ft_strdup("\2");
	else if (str[0] == '?')
		res = ft_itoa(ms()->status);
	else if (str[0] == '"' || str[0] == '\'')
		res = ft_strdup(str);
	else if (get_env(str))
		res = ft_strdup(get_env(str)->data);
	else
		res = ft_strdup("");
	if (ms()->debug)
	{
		prt("\033[1;34m");
		prt("Expander: busca = $%s, resultado = %s\n", str, res);
		prt("\033[0m");
	}
	free(str);
	return (res);
}

bool	has_quote(char c, char prev)
{
	static bool	quote;

	if (c == '\'' || c == '"')
	{
		if (c == '"')
			quote = true;
		if (quote && prev && prev != '\0' && prev != '\'')
			quote = false;
	}
	return (quote);
}

void	expand_str(char **str, int i, int j)
{
	char	*ex_str;

	if (j == i + 1)
		j++;
	ex_str = expansion(ft_substr(*str, i + 1, j - i - 1));
	ft_strrep(str, i, j, ex_str);
	free(ex_str);
}

char	*expander(char *str)
{
	int	i;
	int	j;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			if ((i > 0 && has_quote(str[i], str[i - 1])))
				break ;
			j = i;
			while (str[++j])
				if (!ft_isalnum(str[j]) && str[j] != '_')
					break ;
			expand_str(&str, i, j);
			i = 0;
		}
	}
	search_and_replace(str, '\2', '$');
	return (str);
}
