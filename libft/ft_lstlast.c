/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 14:31:42 by analexan          #+#    #+#             */
/*   Updated: 2023/04/28 13:58:08 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
#include <stdio.h>
#include "printlist.c"
int	main(void)
{
 	t_list *l = ((void*)0);
 	t_list *expected;
 	t_list *actual;
 
 	expected = ((void*)0);
 	actual = ft_lstlast(l);

	printlist(expected);
	printlist(ft_lstlast(l));
	return (0);
}
*/