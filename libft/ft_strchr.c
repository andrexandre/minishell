/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:39:54 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 17:12:40 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	char str[] = "h\0wdy";
	char cc = '\0';

	printf("String: %s\n", str);
	printf("M-> Pointer: %s\n", ft_strchr(str, cc));
	printf("O-> Pointer: %s\n", strchr(str, cc));
	return (0);
}
*/