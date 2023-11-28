/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 18:25:38 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// recieves a string of the form "name=data" and adds it to the ep
void	ep_export_value(char *str)
{
	t_eplist	*curr;
	char		*name;

	name = ft_substr(str, 0,
			ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	curr = get_env(name);
	if (!curr)
		ep_ladd_back(&var()->epl, ep_lnew(str));
	else
	{
		free(curr->str);
		curr->str = ft_strdup(str);
		curr->data = ft_strchr(curr->str, '=') + 1;
	}
	free(name);
}

// searches for the string name in the ep and changes its value to data
void	ep_change_value(char *name, char *data)
{
	char		*str;
	char		*temp;

	temp = ft_strjoin(name, "=");
	str = ft_strjoin(temp, data);
	free(temp);
	ep_export_value(str);
	free(str);
}

// recieves the name of the env to search ex: "HOME"
t_eplist	*get_env(char *name)
{
	t_eplist	*curr;

	curr = var()->epl;
	while (curr)
	{
		if (!ft_strcmp(curr->name, name))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}
