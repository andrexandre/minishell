/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2024/01/13 12:52:41 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// searches for the string name in the envp and returns eplist if it exists
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

void	ep_ladd_back(t_eplist **lst, t_eplist *node)
{
	t_eplist	*last;

	last = *lst;
	while (last->next)
		last = last->next;
	last->next = node;
	node->prev = last;
}

void	ep_lnew(t_eplist **lst, char *str)
{
	t_eplist	*node;

	node = ft_calloc(1, sizeof(t_eplist));
	if (!node || !str)
		return (free(node));
	node->str = ft_strdup(str);
	if (!node->str)
		return ;
			node->name = ft_substr(node->str, 0,
				ft_strlen(node->str) - ft_strlen(ft_strchr(node->str, '=')));
		if (ft_strchr(node->str, '='))
		node->data = ft_strchr(node->str, '=') + 1;
	else
		node->data = NULL;
	node->next = NULL;
	node->prev = NULL;
	if (!*lst)
		*lst = node;
	else
		ep_ladd_back(lst, node);
}

void	ep_ldelone(t_eplist *lst)
{
	if (!lst)
		return ;
	free(lst->str);
	free(lst->name);
	free(lst);
	lst = NULL;
}

void	ep_lclear(t_eplist **lst)
{
	t_eplist	*curr;

	while (*lst)
	{
		curr = (*lst)->next;
		ep_ldelone(*lst);
		*lst = curr;
	}
}
