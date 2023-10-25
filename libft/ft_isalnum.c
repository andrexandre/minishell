/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:08:12 by analexan          #+#    #+#             */
/*   Updated: 2023/04/21 18:40:53 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((!(c < 'A' || c > 'Z') || !(c < 'a' || c > 'z')) || !(c < '0'
			|| c > '9'))
		return (8);
	return (0);
}

/*
#include <ctype.h>
#include <stdio.h>
int	main(void)
{
	char	c = 'a';
	
	printf("%c is %d\n", c, ft_isalnum(c));
	printf("%c is %d\n", c, isalnum(c));
	return (0);
}
*/