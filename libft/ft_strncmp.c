/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:20:16 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 17:22:49 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < n - 1)
		i++;
	if (n == 0)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
/*
#include <stdio.h>
int	main(void)
{
	
	char str1[] = "howdy";
	char str2[] = "howdi";
	int n = 4;

	printf("1st string: %s\n2nd string: %s\n", str1, str2);
	printf("M:\nUntil the %i char. Int: %i\n", n, ft_strncmp(str1, str2, n));
	n = 5;
	printf("Until the %i char. Int: %i\n", n, ft_strncmp(str1, str2, n));
	n = 4;
	printf("O:\nUntil the %i char. Int: %i\n", n, strncmp(str1, str2, n));
	n = 5;
	printf("Until the %i char. Int: %i\n", n, strncmp(str1, str2, n));
	
	printf("R: %i\n", ft_strncmp("test\200", "test\0", 6));
	return (0);
}
*/