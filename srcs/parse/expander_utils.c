/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 15:22:53 by analexan          #+#    #+#             */
/*   Updated: 2024/01/12 21:17:15 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_char(char *str, int c)
{
	char	*new;
	int		i;

	i = ft_strlen(str);
	new = ft_calloc(i + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = -1;
	while (*str)
	{
		if (*str != c)
			new[++i] = *str;
		str++;
	}
	new[i + 1] = '\0';
	return (new);
}
