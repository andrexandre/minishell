/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 10:35:24 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 17:48:56 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}
/*
#include <stdio.h>
#include "printlist.c"
#include "ft_lstadd_front.c"
#include "ft_lstnew.c"
#include "ft_itoa.c"
int	main(void)
{
	t_list	*head;
	t_list	*llst = NULL;
	head = llst;
	llst = ft_lstnew("asd");
	int i = 0;
	while (i < 5)
	{
		llst = ft_lstnew(ft_itoa(i));
		ft_lstadd_front(&head, llst);
		i++;
	}
	printlist(head);
	printf("size: %i\n", ft_lstsize(head));
	return (0);
}
*/