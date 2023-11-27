/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/11/22 15:28:02 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_eplist	*ep_lnew(char *str)
{
	t_eplist	*node;

	node = (t_eplist *)ft_calloc(1, sizeof(t_eplist));
	if (!node)
		return (NULL);
	node->str = ft_strdup(str);
	if (str)
	{
		node->name = ft_substr(node->str, 0,
				ft_strlen(node->str) - ft_strlen(ft_strchr(node->str, '=')));
		node->data = ft_strchr(node->str, '=') + 1;
	}
	else
	{
		node->name = NULL;
		node->data = NULL;
	}
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	ep_ladd_back(t_eplist **lst, t_eplist *new)
{
	t_eplist	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
		new->prev = last;
	}
}

void	ep_ldelone(t_eplist *lst)
{
	if (!lst)
		return ;
	free(lst->str);
	free(lst->name);
	free(lst);
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
	*lst = NULL;
}

void	print_eplst(t_eplist *lst)
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
		ep_ladd_back(&var()->epl, ep_lnew(ep[i]));
	lst = var()->epl;
		
	ep_export_value("ASD=ASD");
	
	if (ac == 2 && get_env(av[1]))
		prt("[%s]\n", get_env(av[1])->str);
	ep_lclear(&var()->epl);
	return (0);
}
*/