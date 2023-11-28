/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 18:28:23 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cd(char *str)
{
	t_eplist	*new;
	char		*cwd;

	cwd = NULL;
	if (!chdir(str))
	{
		new = get_env("PWD");
		if (new)
			ep_change_value("OLDPWD", new->data);
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			perror("getcwd");
			return (1);
		}
		ep_change_value("PWD", cwd);
		free(cwd);
	}
	else
	{
		perror(str);
		return (1);
	}
	return (0);
}

int	run_cd(void)
{
	char		*str;

	if (!var()->words->cmds[1])
	{
		if (!get_env("HOME"))
		{
			prt("cd: HOME not set\n");
			return (1);
		}
		else
			str = get_env("HOME")->data;
	}
	else if (var()->words->cmds[2])
	{
		prt("cd: too many arguments\n");
		return (1);
	}
	else
		str = var()->words->cmds[1];
	return (exec_cd(str));
}
