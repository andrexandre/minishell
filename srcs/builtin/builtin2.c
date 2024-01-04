/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2024/01/04 18:46:47 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// receives the name of the env to search ex: "HOME"
t_eplist	*get_env(char *name)
{
	t_eplist	*curr;

	curr = ms()->epl;
	if (!ft_strcmp(name, "_"))
		return (NULL);
	while (curr)
	{
		if (!ft_strcmp(curr->name, name))
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

int	run_exit(void)
{
	int	i;

	if (isatty(STDIN_FILENO))
		dprt(2, "exit\n");
	ms()->running = 0;
	if (ms()->words->cmds[1])
	{
		i = -1;
		while (ms()->words->cmds[1][++i])
		{
			if (!ft_isdigit(ms()->words->cmds[1][i]) &&
				ms()->words->cmds[1][i] != '-')
			{
				dprt(2, "minishell: exit: %s: numeric argument required\n",
					ms()->words->cmds[1]);
				return (2);
			}
		}
		if (ms()->words->cmds[2])
		{
			dprt(2, "minishell: exit: too many arguments\n");
			ms()->running = 1;
			return (1);
		}
		return (ft_atoll(ms()->words->cmds[1]));
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
			perror("getcwd");
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

int	run_cd(void)
{
	char		*str;

	if (!ms()->words->cmds[1])
	{
		if (!get_env("HOME"))
		{
			dprt(2, "minishell: cd: HOME not set\n");
			return (1);
		}
		else
			str = get_env("HOME")->data;
	}
	else if (ms()->words->cmds[2])
	{
		dprt(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	else
		str = ms()->words->cmds[1];
	return (exec_cd(str));
}
