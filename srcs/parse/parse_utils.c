/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:14:28 by jealves-          #+#    #+#             */
/*   Updated: 2023/12/06 22:02:55 by jealves-         ###   ########.fr       */
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

void	search_and_remove(char *str, char target)
{
	char	*dest;

	dest = str;
	while (*str != '\0')
	{
		if (*str != target)
		{
			*dest = *str;
			dest++;
		}
		str++;
	}
	*dest = '\0';
}

int	ft_strlen_matrix(char **str)
{
	int	i;

	i = 0;
	while (str != NULL && str[i] != NULL)
		i++;
	return (i);
}

int	count_to_pipe(t_list *words)
{
	int		i;
	t_list	*cur;

	i = 1;
	cur = words;
	while (cur && cur->type != PIPE)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}
