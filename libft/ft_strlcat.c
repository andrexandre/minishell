/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 13:02:49 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 13:19:10 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	size_t			dlen;
	size_t			slen;

	i = 0;
	while (dst[i])
		i++;
	dlen = i;
	i = 0;
	while (src[i])
		i++;
	slen = i;
	i = dlen;
	if (size < dlen || size == 0)
		return (slen + size);
	while (src[i - dlen] && i < size - 1)
	{
		dst[i] = src[i - dlen];
		i++;
	}
	dst[i] = '\0';
	return (slen + dlen);
}
/*
// cc ft_strlcat.c -lbsd && ./a.out
#include <stdio.h>
int	main(void)
{
	char	str[] = "Howdy";
	//char	sstr[] = "Howdy";
	char	dstr[] = "not_howdy";
	char	ddstr[] = "not_howdy";

	int size = 20;
	printf("before:\nM-> String: %s\nO-> String: %s\nafter:\n", dstr, ddstr);
	printf("M-> String: %s Lenght: %zu\n", dstr, ft_strlcat(dstr, str, size));
	//printf("O-> String: %s Lenght: %u\n", ddstr, strlcat(ddstr, sstr, size));
	return (0);
}
*/