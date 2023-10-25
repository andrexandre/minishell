/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:39:54 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 17:13:18 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	int				j;
	char			*bptr;
	char			*lptr;

	bptr = (char *)big;
	lptr = (char *)little;
	i = 0;
	j = 0;
	if (!lptr[i])
		return (bptr);
	while (bptr[i])
	{
		while (bptr[i + j] && bptr[i + j] == lptr[j] && i + j < len)
			if (!lptr[j++ + 1])
				return (&bptr[i]);
		j = 0;
		i++;
	}
	return (0);
}
/*
#include <string.h>
#include <stdio.h>
int	main(void)
{
	char str[] = "not_ho_howdyyy";
	char goal[] = "howdy";

	printf("String: %s\n", ft_strnstr(str, goal, 11));
	printf("String: %s\n", ft_strnstr(str, goal, 12));
	// orginal function not working
	//printf("String: %s\n", strnstr(str, goal, 12));
	return (0);
}
*/