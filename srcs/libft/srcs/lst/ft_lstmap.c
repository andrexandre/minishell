/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 22:30:50 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/08 15:59:47 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *))
{
	t_list	*head;
	t_list	*current;
	t_list	*content;

	if (!lst)
		return (NULL);
	head = NULL;
	while (lst)
	{
		content = f(lst);
		current = ft_lstnew(content->str, content->cmds, content->type);
		if (!current)
		{
			del(content);
			ft_lstclear(&head);
			return (NULL);
		}
		ft_lstadd_back(&head, current);
		lst = lst->next;
	}
	return (head);
}
