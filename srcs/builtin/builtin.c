/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/12/20 18:06:13 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if bool 0 print newline if 1 dont print
int	run_echo(void)
{
	int		i;
	int		j;
	int		nl;
	char	*str;

	i = 0;
	nl = 1;
	while (ms()->words->cmds[++i])
	{
		str = ms()->words->cmds[i];
		if (str[0] != '-')
			break ;
		if (!str[0])
			break ;
		j = 1;
		while (str[j])
		{
			if (str[j] != 'n')
				break ;
			j++;
		}
		if (str[j] && str[j] != 'n')
			break ;
		nl = 0;
	}
	prt_strs(ms()->words->cmds + i, nl);
	return (0);
}

int	prt_eplst(void)
{
	t_eplist *lst;

	lst = ms()->epl;
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
	int			status;

	status = 0;
	curr = ms()->epl;
	if (!ms()->words->cmds[1])
	{
		while (curr)
		{
			prt("declare -x %s=\"%s\"\n", curr->name, curr->data);
			curr = curr->next;
		}
	}
	i = 1;
	while (ms()->words->cmds[i])
	{
		if (ft_strchr(ms()->words->cmds[i], '='))
			if (ep_export_value(ms()->words->cmds[i]))
				status = 1;
		i++;
	}
	return (status);
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
	while (ms()->words->cmds[i])
	{
		if (ft_strchr(ms()->words->cmds[i], '='))
		{
			prt("unset: `%s': not a valid identifier\n", ms()->words->cmds[i]);
			exit_code = 1;
			i++;
			continue ;
		}
		curr = get_env(ms()->words->cmds[i]);
		if (curr)
		{
			if (!curr->prev)
				ms()->epl = curr->next;
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
