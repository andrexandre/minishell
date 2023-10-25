/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 13:36:34 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 18:41:42 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned int	i;
	char			*ptr;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (ptr);
}

/*
#include <stdio.h>
int	main(void)
{
	char	c = 'a';
	char	str[] = "howdy1";
	char	str2[] = "howdy2";

	printf("Before:\n");
	printf("M: %s\n", str);
	printf("O: %s\n", str2);
	printf("Ptr's:\n");
	printf("M: %p\nO: %p\n", ft_memset(str, c, 3), memset(str2, c, 3));
	printf("After:\n");
	printf("M: %s\n", str);
	printf("O: %s\n", str2);
	return (0);
}
*/