/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:03:47 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 19:12:14 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}

/*
#include "ft_lstnew.c"
#include <stdio.h>
int	main(void)
{
	t_list	*head;
	t_list	*lst;
	t_list	*l1;

	l1 = ft_lstnew("l1: hi");
	lst = ft_lstnew("lst: howdy");
	head = lst;
	ft_lstadd_front(&head, l1);
	printf("str: '%s'\n", (char *)head->content);
	return (0);
}
*/