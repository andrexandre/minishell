/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/05 22:42:33 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_scnd_token(char old, char c)
{
	if (old == '|')
		return (false);
	if (c == '<' || c == '>')
		return (true);
	return (false);
}

bool	is_token(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

char	*space_token(char *str)
{
	int		i;
	int		j;
	char	*space;
	char	*result;

	i = -1;
	result = ft_strdup(str);
	while (result[++i])
	{
		if (is_token(result[i]))
		{
			j = i;
			while (result[++j])
			{
				if (!is_scnd_token(result[i], result[j]))
					break ;
			}
			space = ft_strdup(" ");
			ft_strrep(&result, j, j, space);
			ft_strrep(&result, i, i, space);
			free(space);
			i = j;
		}
	}
	return (result);
}
