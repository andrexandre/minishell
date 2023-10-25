/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 17:14:25 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 15:01:30 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	i = 0;
	while (src[i])
		i++;
	return (i);
}

/*
//prints 1 less than size because strlcpy does that and then '\0'
// function has to be tested without -w -w -w and with -lbsd
#include <stdio.h>
#include <string.h>
int	main(void)
{
	int		size = 5;
	char	str[] = "";
	char	dstr[size];
	char	sstr[] = "";
	char	ddstr[size];

	printf("M-> str:  %s dstr:  %s Lenght: %ld\n", str, dstr
	, ft_strlcpy(dstr, str, size));
	printf("O-> sstr: %s ddstr: %s Lenght: %d\n", sstr, ddstr
	, strlcpy(ddstr, sstr, size));
	write(1, dstr, size);
	printf("\n");
	write(1, ddstr, size);
	return (0);
}
*/