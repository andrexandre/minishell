/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 14:59:02 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_echo(void)
{
	int		i;
	char	*str;

	i = 2;
	if (var()->lst_lexer->next && !ft_strncmp(var()->lst_lexer->next->str, "-n", 2))
	{
		str = var()->lst_lexer->next->str;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			prt_lst(var()->lst_lexer->next->next, 1);
		else
			prt_lst(var()->lst_lexer->next, 0);
	}
	else
		prt_lst(var()->lst_lexer->next, 0);
	return (0);
}

int	run_env(void)
{
	if (!var()->lst_lexer->next)
		print_eplst(var()->epl);
	return (0);
}

int	run_export(void)
{
	t_eplist	*curr;
	t_list		*new;

	curr = var()->epl;
	if (!var()->lst_lexer->next)
	{
		while (curr)
		{
			prt("declare -x %s=\"%s\"\n", curr->name, curr->data);
			curr = curr->next;
		}
		return (0);
	}
	new = var()->lst_lexer;
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
	new = var()->lst_lexer->next;
	while (new)
	{
		if (ft_strchr(new->str, '='))
		{
			prt("unset: `%s': not a valid identifier\n", new->str);
			exit_code = 1;
			new = new->next;
			continue ;
		}
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
	return (exit_code);
}
