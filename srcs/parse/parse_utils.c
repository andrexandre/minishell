/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:14:28 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/13 00:54:02 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	search_and_replace(char *str, char src, char dest)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == src)
			str[i] = dest;
		i++;
	}
}

bool	inside_quote(char str, char *target, char *quote)
{
	int	i;

	i = 0;
	if (*quote == '\0')
	{
		while (target[i])
		{
			if (str == target[i])
				*quote = target[i];
			i++;
		}
	}
	else
	{
		while (target[i])
		{
			if (str == *quote)
			{
				*quote = '\0';
				return (false);
			}
			i++;
		}
	}
	return (str != *quote);
}

char	*create_search_and_remove(char *str, char *target, char *quote, int j)
{
	char	*dest;
	int		i;

	dest = ft_calloc(sizeof(char), j + 1);
	j = 0;
	i = 0;
	while (str[i])
	{
		if (inside_quote(str[i], target, quote))
		{
			dest[j] = str[i];
			j++;
		}
		i++;
	}
	dest[j] = '\0';
	search_and_replace(dest, '\4', '\'');
	search_and_replace(dest, '\5', '"');
	free(str);
	return (dest);
}

char	*search_and_remove(char *str, char *target)
{
	char	quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	quote = '\0';
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (inside_quote(str[i], target, &quote))
			j++;
		i++;
	}
	return (create_search_and_remove(str, target, &quote, j));
}

int	ft_strlen_matrix(char **str)
{
	int	i;

	i = 0;
	while (str != NULL && str[i] != NULL)
		i++;
	return (i);
}
