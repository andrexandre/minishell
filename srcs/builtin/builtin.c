/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/23 22:10:13 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_echo(void)
{
	int		i;
	char	*str;

	i = 2;
	if (var()->words->next && !ft_strncmp(var()->words->next->str, "-n", 2))
	{
		str = var()->words->next->str;
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
	if (var()->words->next)
		prt("env: too many arguments\n");
	else
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
		if (ft_strchr(new->str, '='))
			ep_export_value(new->str);
		new = new->next;
	}
	return (0);
}

int	run_pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (0);
	}
	prt("%s\n", cwd);
	free(cwd);
	return (0);
}

int	run_unset(void)
{
	t_eplist	*curr;
	t_list		*new;

	new = var()->words->next;
	while (new)
	{
		if (ft_strchr(new->str, '='))
			prt("unset: `%s': not a valid identifier\n", new->str);
		curr = get_env(new->str);
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
	return (0);
}
