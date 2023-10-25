/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:59:15 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 17:37:23 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned int		i;
	unsigned char		*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		if ((unsigned char)ptr[i] == (unsigned char)c && i < n)
		{
			return (&ptr[i]);
		}
		i++;
	}
	return (0);
}

/*
#include <stdio.h>
int	main(void)
{
	// char str[] = "not_howdy";
	char cc = '\200';
	char *str = "/|\x12\xff\x09\x42\2002\42|\\";
 	int size = 10;
	printf("String: %s\n", str);
	char *ft_ptr = ft_memchr(str, cc, size);
	char *ptr = memchr(str, cc, size);
	printf("M-> Pointer: %s\n", ft_ptr);
	printf("O-> Pointer: %s\n", ptr);
	return (0);
}
*/
