/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 17:39:45 by analexan          #+#    #+#             */
/*   Updated: 2023/04/27 18:06:19 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*str;

	i = 0;
	while (s[i])
		i++;
	str = malloc(i + 1);
	if (!str)
		return (0);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = 0;
	return (str);
}
/*
static char	ft_rnd(unsigned int i, char c)
{
	return (i + c);
}
#include <stdio.h>
int	main(void)
{
	char	str[] = "howdy";
	char	*ptr = ft_strmapi(str, ft_rnd);

	printf("str:    %s\nft_str: %s\n", str, ptr);
	free(ptr);
	return (0);
}
*/