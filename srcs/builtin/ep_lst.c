/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ep_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 10:49:51 by analexan          #+#    #+#             */
/*   Updated: 2023/12/06 13:32:45 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// receives a string of the form "name=data" and adds it to the ep
void	ep_export_value(char *str)
{
	t_eplist	*curr;
	char		*name;

	name = ft_substr(str, 0,
			ft_strlen(str) - ft_strlen(ft_strchr(str, '=')));
	if (!ft_strcmp(name, "_"))
	{
		free(name);
		return ;
	}
	curr = get_env(name);
	if (!curr)
		ep_lnew_add_back(&var()->epl, str);
	else
	{
		free(curr->str);
		curr->str = ft_strdup(str);
		curr->data = ft_strchr(curr->str, '=') + 1;
	}
	free(name);
}

// searches for the string name in the ep and changes its value to data
void	ep_change_value(char *name, char *data)
{
	char		*str;
	char		*temp;

	temp = ft_strjoin(name, "=");
	str = ft_strjoin(temp, data);
	free(temp);
	ep_export_value(str);
	free(str);
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