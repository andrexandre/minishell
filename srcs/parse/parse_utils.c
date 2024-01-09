/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:14:28 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/08 20:11:17 by jealves-         ###   ########.fr       */
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

char	ft_get_first_target(char *str, char *target)
{
	char	fc;
	int		i;
	int		j;

	i = 0;
	fc = '\2';
	while (str[i])
	{
		j = 0;
		while (target[j])
		{
			if (str[i] == target[j])
			{
				fc = target[j];
				break ;
			}
			j++;
		}
		if (fc != '\2')
			break ;
		i++;
	}
	return (fc);
}

void	search_and_remove(char *str, char *target)
{
	char	*dest;
	char	first_target;

	dest = str;
	first_target = ft_get_first_target(str, target);
	if (!str)
		return ;
	while (*str != '\0')
	{
		if (*str != first_target)
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
