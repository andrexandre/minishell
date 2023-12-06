/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/12/06 13:30:12 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_echo(void)
{
	int		i;
	char	*str;

	i = 2;
	if (var()->words->cmds[1] && !ft_strncmp(var()->words->cmds[1], "-n", 2))
	{
		str = var()->words->cmds[1];
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			prt_strs(var()->words->cmds + 2, 1);
		else
			prt_strs(var()->words->cmds + 1, 0);
	}
	else
		prt_strs(var()->words->cmds + 1, 0);
	return (0);
}

int	prt_eplst(void)
{
	t_eplist *lst;

	lst = var()->epl;
	while (lst)
	{
		prt("%s\n", lst->str);
		lst = lst->next;
	}
	return (0);
}

int	run_export(void)
{
	t_eplist	*curr;
	int			i;

	curr = var()->epl;
	if (!var()->words->cmds[1])
	{
		while (curr)
		{
			prt("declare -x %s=\"%s\"\n", curr->name, curr->data);
			curr = curr->next;
		}
	}
	i = 1;
	while (var()->words->cmds[i])
	{
		if (ft_strchr(var()->words->cmds[i], '='))
			ep_export_value(var()->words->cmds[i]);
		i++;
	}
	return (0);
}

int	run_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	prt("%s\n", cwd);
	free(cwd);
	return (0);
}

int	run_unset(void)
{
	t_eplist	*curr;
	int			i;
	int			exit_code;

	exit_code = 0;
	i = 1;
	while (var()->words->cmds[i])
	{
		if (ft_strchr(var()->words->cmds[i], '='))
		{
			prt("unset: `%s': not a valid identifier\n", var()->words->cmds[i]);
			exit_code = 1;
			i++;
			continue ;
		}
		curr = get_env(var()->words->cmds[i]);
		if (curr)
		{
			if (!curr->prev)
				var()->epl = curr->next;
			else if (!curr->next)
				curr->prev->next = NULL;
			else
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}
			ep_ldelone(curr);
		}
		i++;
	}
	return (exit_code);
}
