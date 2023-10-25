/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:34:50 by analexan          #+#    #+#             */
/*   Updated: 2023/04/22 14:03:02 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const char		*sptr;
	char			*dptr;

	if (!dest && !src)
		return (NULL);
	if (dest < src)
		ft_memcpy(dest, src, n);
	else
	{
		sptr = src + n - 1;
		dptr = dest + n - 1;
		while (n-- > 0)
			*dptr-- = *sptr--;
	}
	return (dest);
}

/*
//If dest is less than src, it means that the source address 
//is below the destination address in memory
#include <unistd.h>
#include <stdio.h>
int	main(void)
{
	char	dest[] = "lorem ipum dolor sit a";
	//char    *src = dest;
	ft_memmove(dest, "con\0sec\0\0te\0tur", 10);
	//exe | cat -e
	write(1, dest, 22);
	printf("\n");
	return (0);
}
*/
