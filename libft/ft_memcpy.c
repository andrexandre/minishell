/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 19:08:37 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 11:20:20 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int	i;
	char			*sptr;
	char			*dptr;

	i = 0;
	if (!dest && !src)
		return (NULL);
	sptr = (char *)src;
	dptr = (char *)dest;
	while (i < n)
	{
		dptr[i] = sptr[i];
		i++;
	}
	return (dptr);
}
/*
#include <stdio.h>
int	main(void)
{
	char	str[] = "howdy1";
	char	str2[] = "aaaaaa";
	int		v;

	printf("My function (0) or Original function (1):");
	scanf("%d", &v);
	printf("Before: %s\n", str);
	if (v == 0)
		printf("M: %p\n", ft_memcpy(str2, str, 3));
	else
		printf("O: %p\n", memcpy(str2, str, 3));
	printf("After:  %s\n", str2);
	return (0);
}
*/