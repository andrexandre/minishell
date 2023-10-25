/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:59:15 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 17:54:23 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned int		i;
	unsigned char		*ptr1;
	unsigned char		*ptr2;

	i = 0;
	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (ptr1[i] == ptr2[i] && i < n - 1)
		i++;
	return (ptr1[i] - ptr2[i]);
}

/*
#include <stdio.h>
int	main(void)
{
	char str1[] = "howdy";
	char str2[] = "howdi";
	int n = 4;

	printf("1st string: %s\n2nd string: %s\n", str1, str2);
	printf("M:\nUntil the %i char. Int: %i\n", n, ft_memcmp(str1, str2, n));
	n = 5;
	printf("Until the %i char. Int: %i\n", n, ft_memcmp(str1, str2, n));
	n = 4;
	printf("O:\nUntil the %i char. Int: %i\n", n, memcmp(str1, str2, n));
	n = 5;
	printf("Until the %i char. Int: %i\n", n, memcmp(str1, str2, n));
	return (0);
}
*/