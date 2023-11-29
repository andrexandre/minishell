/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/11/29 18:58:47 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// receives the name of the env to search ex: "HOME"
t_eplist	*get_env(char *name)
{
	t_eplist	*curr;

	curr = var()->epl;
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

void	ep_lnew_add_back(t_eplist **lst, char *str)
{
	t_eplist	*node;
	t_eplist	*last;

	node = ft_calloc(1, sizeof(t_eplist));
	if (!node || !str)
		return ;
	node->str = ft_strdup(str);
	node->name = ft_substr(node->str, 0,
			ft_strlen(node->str) - ft_strlen(ft_strchr(node->str, '=')));
	node->data = ft_strchr(node->str, '=') + 1;
	node->next = NULL;
	node->prev = NULL;
	if (!*lst)
		*lst = node;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = node;
		node->prev = last;
	}
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

void	prt_eplst(t_eplist *lst)
{
	while (lst)
	{
		prt("%s\n", lst->str);
		lst = lst->next;
	}
}

/*
int	main(int ac, char **av, char **ep)
{
	t_eplist	*lst;
	int			i;

	i = -1;
	while (ep[++i])
		ep_lnew_add_back(&var()->epl, ep[i]);
	lst = var()->epl;
		
	ep_export_value("ASD=ASD");
	
	if (ac == 2 && get_env(av[1]))
		prt("[%s]\n", get_env(av[1])->str);
	ep_lclear(&var()->epl);
	return (0);
}
*/