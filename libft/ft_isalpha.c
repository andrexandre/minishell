/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:08:12 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 19:01:28 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if (!(c < 'A' || c > 'Z') || !(c < 'a' || c > 'z'))
		return (1024);
	return (0);
}

/*
#include <ctype.h>
#include <stdio.h>
int	main(void)
{
	char	c = 'a';

	printf("%c is %d\n", c, ft_isalpha(c));
	printf("%c is %d\n", c, isalpha(c));
	return (0);
}
*/