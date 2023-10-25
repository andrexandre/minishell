/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:08:12 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 18:41:08 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c < ' ' || c > '~')
		return (0);
	return (16384);
}

/*
#include <ctype.h>
#include <stdio.h>
int	main(void)
{
	char	c = 'a';

	printf("%c is %d\n", c, ft_isprint(c));
	printf("%c is %d\n", c, isprint(c));
	return (0);
}
*/