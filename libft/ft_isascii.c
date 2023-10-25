/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:08:12 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 18:41:01 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c < 0 || c > 127)
		return (0);
	return (1);
}

/*
#include <ctype.h>
#include <stdio.h>
int	main(void)
{
	int	c = 'a';

	printf("%c is %d\n", c, ft_isascii(c));
	printf("%c is %d\n", c, isascii(c));
	return (0);
}
*/