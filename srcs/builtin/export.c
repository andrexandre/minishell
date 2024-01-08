/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:44:42 by analexan          #+#    #+#             */
/*   Updated: 2024/01/08 17:16:23 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_error(char *name, char *str)
{
	int	i;

	i = -1;
	if (ft_isdigit(name[0]))
	{
		dprt(2, "minishell: export: `%s': not a valid identifier\n", str);
		free(name);
		return (1);
	}
	while (name[++i])
		if (!ft_isalnum(name[i]) && name[i] != '_')
			break ;
	if (!name[i])
		return (0);
	dprt(2, "minishell: export: `%s': not a valid identifier\n", str);
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
	if (export_error(name, str))
		return (1);
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

int	run_export(void)
{
	t_eplist	*curr;
	int			i;
	int			status;

	status = 0;
	curr = ms()->epl;
	if (!ms()->words->cmds[1])
	{
		while (curr)
		{
			if (ft_strchr(curr->str, '='))
				prt("declare -x %s=\"%s\"\n", curr->name, curr->data);
			else
				prt("declare -x %s\n", curr->name);
			curr = curr->next;
		}
	}
	i = 1;
	while (ms()->words->cmds[i])
	{
		if (ep_export_value(ms()->words->cmds[i]))
			status = 1;
		i++;
	}
	return (status);
}
