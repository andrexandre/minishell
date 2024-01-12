/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2024/01/12 12:58:39 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if bool 0 print newline if 1 don't print
int	run_echo(t_list *curr)
{
	int		i;
	int		j;
	int		nl;
	char	*str;

	i = 0;
	nl = 1;
	while (curr->cmds[++i])
	{
		str = curr->cmds[i];
		if (str[0] != '-' || !str[0])
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
	prt_strs(curr->cmds + i, nl);
	return (0);
}

int	prt_eplst(void)
{
	t_eplist	*lst;

	lst = ms()->epl;
	while (lst)
	{
		if (ft_strchr(lst->str, '='))
			prt("%s\n", lst->str);
		lst = lst->next;
	}
	return (0);
}

int	run_unset(t_list *curr)
{
	t_eplist	*to_change;
	int			i;
	int			exit_code;

	exit_code = 0;
	i = 1;
	while (curr->cmds[i])
	{
		to_change = get_env(curr->cmds[i]);
		if (to_change)
		{
			if (!to_change->prev)
				ms()->epl = to_change->next;
			else if (!to_change->next)
				to_change->prev->next = NULL;
			else
			{
				to_change->prev->next = to_change->next;
				to_change->next->prev = to_change->prev;
			}
			ep_ldelone(to_change);
		}
		i++;
	}
	return (exit_code);
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
