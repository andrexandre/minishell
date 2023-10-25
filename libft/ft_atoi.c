/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:56:08 by analexan          #+#    #+#             */
/*   Updated: 2023/04/28 12:43:27 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	mc;
	int	n;

	i = 0;
	mc = 1;
	n = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	while ((nptr[i] == '-' || nptr[i] == '+') && ft_isdigit(nptr[i + 1]))
	{
		if (nptr[i] == '-')
			mc *= -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		n = n * 10;
		n = n + (nptr[i] - '0');
		i++;
	}
	return (n * mc);
}
/*
#include <stdio.h>
#include <stdlib.h>
int	main(void)
{
	char	str[] = "-1234a5";
	char	str1[] = "+1234a5";
	char	str2[] = "--+-1234a5";
	
	printf("Str: %s Str: %s Str: %s\n", str, str1, str2);
	printf("  M: %i     M: %i      M: %i\n", ft_atoi(str)
	, ft_atoi(str1), ft_atoi(str2));
	printf("  O: %i     O: %i      O: %i\n", atoi(str)
	, atoi(str1), atoi(str2));
	return (0);
}
*/