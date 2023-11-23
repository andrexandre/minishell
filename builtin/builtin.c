/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/23 19:03:37 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_echo(void)
{
	int		i;
	char	*str;

	i = 2;
	if (var()->words->next && !ft_strncmp(var()->words->next->content, "-n", 2))
	{
		str = var()->words->next->content;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			print_lst(var()->words->next->next, 1);
		else
			print_lst(var()->words->next, 0);
	}
	else
		print_lst(var()->words->next, 0);
	return (0);
}

int	run_env(void)
{
	if (!var()->words->next)
		print_eplst(var()->epl);
	return (0);
}

int	run_export(void)
{
	t_eplist	*curr;
	t_list		*new;

	curr = var()->epl;
	if (!var()->words->next)
	{
		while (curr)
		{
			prt("declare -x %s=\"%s\"\n", curr->name, curr->data);
			curr = curr->next;
		}
		return (0);
	}
	new = var()->words;
	while (new)
	{
		if (ft_strchr(new->content, '='))
			ep_export_value(new->content);
		new = new->next;
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
	t_list		*new;
	int			exit_code;

	exit_code = 0;
	new = var()->words->next;
	while (new)
	{
		if (ft_strchr(new->content, '='))
		{
			prt("unset: `%s': not a valid identifier\n", new->content);
			exit_code = 1;
			new = new->next;
			continue ;
		}
		curr = get_env(new->content);
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
		new = new->next;
	}
	return (exit_code);
}
