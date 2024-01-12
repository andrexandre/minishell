/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2024/01/12 13:55:13 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_error(void)
{
	ms()->running = 1;
	return (dprt(2, "minishell: exit: too many arguments\n"), 1);
}

int	run_exit(t_list *curr)
{
	int	i;

	if (isatty(STDIN_FILENO))
		dprt(2, "exit\n");
	ms()->running = 0;
	if (curr->cmds[1])
	{
		i = -1;
		while (curr->cmds[1][++i])
		{
			if (!ft_isdigit(curr->cmds[1][i])
				&& curr->cmds[1][0] != '-')
				return (dprt(2,
						"minishell: exit: %s: numeric argument required\n",
						curr->cmds[1]), 2);
		}
		if (!curr->cmds[1][0])
			return (dprt(2, "minishell: exit: : numeric argument required\n"),
				2);
		if (curr->cmds[2])
			return (exit_error());
		return (ft_atoll(curr->cmds[1]));
	}
	return (ms()->status);
}

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
			perror("cd: getcwd");
			return (1);
		}
		ep_change_value("PWD", cwd);
		free(cwd);
	}
	else
	{
		dprt(2, "minishell: cd: ");
		perror(str);
		return (1);
	}
	return (0);
}

int	run_cd(t_list *curr)
{
	char		*str;

	if (!curr->cmds[1])
	{
		if (!get_env("HOME"))
		{
			dprt(2, "minishell: cd: HOME not set\n");
			return (1);
		}
		else
			str = get_env("HOME")->data;
	}
	else if (curr->cmds[2])
	{
		dprt(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
		str = curr->cmds[1];
	if (curr->cmds[1] && !curr->cmds[1][0])
		return (0);
	return (exec_cd(str));
}
