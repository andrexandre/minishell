/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:14:53 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 17:49:30 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	del(lst->content);
	free(lst);
}
/*
#include "printlist.c"
#include "ft_lstnew.c"
#include "ft_lstadd_front.c"
void	ft_del(void *content)
{
	free(content);
}
#include <stdio.h>
int	main(void)
{
	t_list	*head;
	t_list	*llst = NULL;

	head = llst;
	llst = ft_lstnew("hi");
	ft_lstadd_front(&head, llst);
	ft_lstdelone(llst, ft_del);
	// printlist(llst);
	// free(llst);
	return (0);
}
*/