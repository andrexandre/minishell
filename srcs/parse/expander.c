/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 10:55:20 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/12 22:24:07 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expansion(char *str)
{
	char	*res;

	if (str[0] == '\0')
		res = ft_strdup("\2");
	else if (str[0] == '?')
		res = ft_itoa(ms()->status);
	else if (str[0] == '"' || str[0] == '\'')
		res = ft_strdup(str);
	else if (get_env(str))
		res = ft_strdup(get_env(str)->data);
	else
		res = ft_strdup("\3");
	if (ms()->debug)
	{
		prt("\033[1;34m");
		prt("Expander: busca = $%s, resultado = %s\n", str, res);
		prt("\033[0m");
	}
	free(str);
	str = NULL;
	return (res);
}

void	has_quote(bool *quote, char *str, int i)
{
	if (str[i] == '\'')
	{
		if (i > 0 && str[i - 1] == '"')
			*quote = false;
		else
			*quote = !*quote;
	}
}

void	expand_str(char **str, int i)
{
	char	*ex_str;
	char	*src;
	int		j;

	src = *str;
	j = i;
	while (src[++j])
	{
		if (!ft_isalnum(src[j]) && src[j] != '_' && src[j] != '?')
			break ;
		else if (src[j] == '?' && j == i + 2)
			break ;
	}
	ex_str = expansion(ft_substr(*str, i + 1, j - i - 1));
	ft_strrep(str, i, j, ex_str);
	free(ex_str);
	ex_str = NULL;
}

char	*expander(char *str)
{
	int		i;
	bool	quote;
	char	*tmp;
	char	*dest;

	quote = false;
	i = -1;
	while (str[++i])
	{
		has_quote(&quote, str, i);
		if (!quote && str[i] == '$')
		{
			expand_str(&str, i);
			i = 0;
		}
	}
	search_and_replace(str, '\2', '$');
	tmp = str;
	str = remove_char(tmp, '\3');
	free(tmp);
	tmp = NULL;
	dest = search_and_remove(str, "'\"");
	return (dest);
}

char	**expander_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		cmd[i] = expander(cmd[i]);
		i++;
	}
	return (cmd);
}
