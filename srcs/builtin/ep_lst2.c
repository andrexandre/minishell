/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/11/29 18:44:02 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// receives a string of the form "name=data" and adds it to the ep
void	ep_export_value(char *str)
{
	t_eplist	*curr;
	char		*name;

	name = ft_substr(str, 0,
			ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	if (!ft_strcmp(name, "_"))
	{
		free(name);
		return ;
	}
	curr = get_env(name);
	if (!curr)
		ep_lnew_add_back(&var()->epl, str);
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
