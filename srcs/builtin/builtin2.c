/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/23 22:28:37 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ep_change_value(char *key, char *value)
{
	//t_eplist	*node;
	char		*str;
	char		*temp;

	//node = get_env(key);
	temp = ft_strjoin(key, "=");
	str = ft_strjoin(temp, value);
	free(temp);
	ep_export_value(str);
	free(str);
}

int	run_cd(void)
{
	t_eplist	*new;
	char		*str;
	char		*cwd;

	cwd = NULL;
	if (!var()->words->next)
	{
		if (!get_env("HOME"))
		{
			prt("cd: HOME not set\n");
			return (0);
		}
		else
			str = get_env("HOME")->data;
	}
	else
		str = var()->words->next->str;
	if (!chdir(str))
	{
		new = get_env("PWD");
		if (new)
		{
			str = ft_strjoin("OLDPWD=", new->data);
			ep_export_value(str);
			free(str);
		}
		cwd = getcwd(cwd, 0);
		if (!cwd)
		{
			perror("getcwd");
			return (0);
		}
		str = ft_strjoin("PWD=", cwd);
		free(cwd);
		ep_export_value(str);
		free(str);
	}
	else
		perror(str);
	return (0);
}
