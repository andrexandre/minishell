/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/11/20 16:38:23 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_eplist	*ep_export_value(char *str)
{
	t_eplist	*curr;
	char		*name;

	name = ft_substr(str, 0,
			ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	curr = get_env(name);
	if (!curr)
		ep_ladd_back(&var()->epl, ep_lnew(ft_strdup(str)));
	else
	{
		free(curr->str);
		curr->str = ft_strdup(str);
		curr->data = ft_strchr(curr->str, '=') + 1;
	}
	curr = get_env(name);
	free(name);
	return (curr);
}

t_eplist	*get_env(char *key)
{
	t_eplist	*curr;

	curr = var()->epl;
	while (curr)
	{
		if (!ft_strcmp(curr->name, key))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}
