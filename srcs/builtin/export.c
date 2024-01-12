/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:44:42 by analexan          #+#    #+#             */
/*   Updated: 2024/01/12 17:24:09 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_error(char *name)
{
	int	i;

	i = -1;
	if (ft_isdigit(name[0]))
	{
		free(name);
		return (1);
	}
	while (name[++i])
		if (!ft_isalnum(name[i]) && name[i] != '_')
			break ;
	if (!name[i])
		return (0);
	free(name);
	return (1);
}

// receives a string of the form "name=data" and adds it to the ep
int	ep_export_value(char *str)
{
	t_eplist	*curr;
	char		*name;

	name = ft_substr(str, 0, ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	if (!name)
		return (1);
	if (export_error(name) || str[0] == '=')
		return (dprt(2, "minishell: export: `%s': not a valid identifier\n", str), 1);
	if (!ft_strcmp(name, "_"))
	{
		free(name);
		return (0);
	}
	curr = get_env(name);
	if (!curr)
		ep_lnew(&ms()->epl, str);
	else if (ft_strchr(str, '='))
	{
		free(curr->str);
		curr->str = ft_strdup(str);
		curr->data = ft_strchr(curr->str, '=') + 1;
	}
	free(name);
	return (0);
}

// searches for the string name in the ep and changes its value to data
int	ep_change_value(char *name, char *data)
{
	char		*str;
	char		*temp;

	temp = ft_strjoin(name, "=");
	str = ft_strjoin(temp, data);
	free(temp);
	if (ep_export_value(str))
		return (1);
	free(str);
	return (0);
}

int	run_export(t_list *curr)
{
	t_eplist	*to_change;
	int			i;
	int			status;

	status = 0;
	to_change = ms()->epl;
	if (!curr->cmds[1])
	{
		while (to_change)
		{
			if (ft_strchr(to_change->str, '='))
				prt("declare -x %s=\"%s\"\n", to_change->name, to_change->data);
			else
				prt("declare -x %s\n", to_change->name);
			to_change = to_change->next;
		}
	}
	i = 1;
	while (curr->cmds[i])
	{
		if (ep_export_value(curr->cmds[i]))
			status = 1;
		i++;
	}
	return (status);
}
