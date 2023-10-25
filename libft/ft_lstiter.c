/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 14:10:49 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 16:42:08 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
void	ft_changetoz(void *content)
{
	char	*ptr = (char *)content;
	ptr[0] = 'z';
}
#include <stdio.h>
#include "printlist.c"
#include "ft_lstnew.c"
#include "ft_lstadd_front.c"
#include "ft_strdup.c"
int	main(void)
{
	t_list	*head;
	t_list	*llst = NULL;

	head = llst;
	int i = 0;
	while (i < 2)
	{
		llst = ft_lstnew(ft_strdup("asd"));
		ft_lstadd_front(&head, llst);
		i++;
	}
	printlist(head);
	ft_lstiter(head, ft_changetoz);
	printlist(head);
	return (0);
}
*/