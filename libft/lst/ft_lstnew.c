/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 22:32:46 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/10 16:14:17 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnewold(void *content)
{
	t_list	*node;

	node = (t_list *)ft_calloc(1, sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (t_list *)ft_calloc(1, sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = ft_strdup(content);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}
