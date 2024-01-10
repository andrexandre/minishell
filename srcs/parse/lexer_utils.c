/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/09 15:26:55 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_scnd_token(char old, char c)
{
	if (old == '|')
		return (false);
	if (old == c && (c == '<' || c == '>'))
		return (true);
	return (false);
}

bool	is_token(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

int	ft_put_space(int i, char **result)
{
	char	*space;
	int		j;
	int		len;
	char	*res;

	res = *result;
	len = ft_strlen(*result);
	j = i + 1;
	while (len > j && res[j])
	{
		if (!is_scnd_token(res[i], res[j]))
			break ;
		j++;
	}
	space = ft_strdup(" ");
	ft_strrep(result, j, j, space);
	ft_strrep(result, i, i, space);
	free(space);
	return (j);
}

char	*space_token(char *str)
{
	int		i;
	char	*result;
	bool	quote;

	i = -1;
	quote = false;
	result = ft_strdup(str);
	while (result[++i])
	{
		if (result[i] == '\'' || result[i] == '"')
			quote = !quote;
		if (is_token(result[i]) && !quote)
		{
			i = ft_put_space(i, &result);
		}
	}
	return (result);
}
