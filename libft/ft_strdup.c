/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:34:10 by analexan          #+#    #+#             */
/*   Updated: 2023/04/24 16:03:59 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;

	i = 0;
	while (s[i])
		i++;
	str = malloc(i + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char str1[] = "howdy";
	char *strm;
	strm = ft_strdup(str1);
	char *stro;
	stro = strdup(str1);
	
	printf("String used: %s\n", str1);
	printf("M: %p\n", strm);
	printf("O: %p\n", stro);
	return (0);
}
*/