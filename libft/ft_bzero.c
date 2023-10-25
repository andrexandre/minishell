/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 18:32:45 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 18:40:16 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned int	i;
	char			*ptr;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
/*
#include <stdio.h>

int	main(void)
{
	char	str[] = "howdy1";
	char	str2[] = "howdy2";

	printf("Before:\n");
	printf("M: %s\n", str);
	printf("O: %s\n", str2);
	ft_bzero(str, 3);
	bzero(str2, 3);
	printf("After:\n");
	printf("M: %s\n", str);
	printf("O: %s\n", str2);
	return (0);
}
*/