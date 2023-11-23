/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:02:07 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/23 22:27:43 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;
	size_t	i;

	i = 0;
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
	{
		return (NULL);
	}
	while (i < new_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
