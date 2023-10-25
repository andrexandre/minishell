/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 11:21:53 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 16:42:14 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	while (*lst)
	{
		temp = *lst;
		*lst = temp->next;
		del(temp->content);
		free(temp);
	}
	*lst = NULL;
}

/*
void	ft_del(void *content)
{
	free(content);
}
#include <stdio.h>
#include "printlist.c"
#include "ft_lstnew.c"
#include "ft_lstadd_front.c"
#include "ft_itoa.c"
int	main(void)
{
	t_list	*head;
	t_list	*llst = NULL;

	head = llst;
	llst = ft_lstnew("hi");
	int i = 0;
	while (i < 2)
	{
		llst = ft_lstnew(ft_itoa(i));
		ft_lstadd_front(&head, llst);
		i++;
	}
	ft_lstclear(&head, ft_del);
	printlist(head);
	return (0);
}
*/