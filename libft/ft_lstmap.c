/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 14:26:32 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 14:22:01 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*temp;
	void	*list_content;

	new_lst = NULL;
	while (lst)
	{
		list_content = f(lst->content);
		temp = ft_lstnew(list_content);
		if (!temp)
		{
			del(list_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, temp);
		lst = lst->next;
	}
	return (new_lst);
}
	// while (lst)
	// {
	// 	mapped_content = f(lst->content);
	// 	node = ft_lstnew(mapped_content);
	// 	if (!node)
	// 	{
	// 		del(mapped_content);
	// 		ft_lstclear(&new_lst, del);
	// 		return (NULL);
	// 	}
	// 	ft_lstadd_back(&new_lst, node);
	// 	lst = lst->next;
	// }

/*
#include "printlist.c"
#include "ft_lstnew.c"
#include "ft_lstclear.c"
#include "ft_lstadd_back.c"
void	*ft_changetoz(void *content)
{
	char	*ptr = (char *)content;

	ptr[0] = 'z';
	return (ptr);
}
void	ft_del(void *content)
{
	free(content);
}
#include <stdio.h>
#include "ft_lstadd_front.c"
#include "ft_strdup.c"
int	main(void)
{
	t_list	*head;
	t_list	*llst = NULL;
	t_list	*headd;
	t_list	*llstt = NULL;
	int		i;

	head = llst;
	headd = llstt;
	llst = ft_lstnew(ft_strdup("hi"));
	i = 0;
	while (i < 2)
	{
		llst = ft_lstnew(ft_strdup("hi"));
		ft_lstadd_front(&head, llst);
		i++;
	}
	printlist(head);
	headd = ft_lstmap(llst, ft_changetoz, ft_del);
	printlist(headd);
	return (0);
}
*/
