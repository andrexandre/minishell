/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:18:24 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 17:54:46 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	temp = ft_lstlast(*lst);
	if (!*lst)
		*lst = new;
	else
		temp->next = new;
}
/*
#include "ft_lstlast.c"
#include "ft_lstnew.c"
#include "ft_strdup.c"
#include "printlist.c"
#include <stdio.h>
int	main(void)
{
 	t_list *l = ((void*)0);
 	t_list *n = ft_lstnew(ft_strdup("OK"));
 
 	ft_lstadd_back(&l, n);
	printlist(l);
	printlist(n);
	return (0);
}
*/