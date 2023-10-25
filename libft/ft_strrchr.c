/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:39:54 by analexan          #+#    #+#             */
/*   Updated: 2023/04/28 12:58:44 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	int		i;

	i = ft_strlen(s);
	while (i > -1)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (0);
}
/*
int	main(void)
{
	char str[] = "not_howdy";
	char cc = 'o';

	printf("String: %s\n", str);
	printf("M-> Pointer: %s\n", ft_strrchr(str, cc));
	printf("O-> Pointer: %s\n", strrchr(str, cc));
	return (0);
}
*/