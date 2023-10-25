/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isdigit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 18:08:12 by analexan          #+#    #+#             */
/*   Updated: 2023/02/05 19:33:45 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
		return (0);
	return (2048);
}

/*
#include <stdio.h>
#include <stdlib.h>
int	main(void)
{	
	// char	c = '1';

	// printf("%c is %d\n", c, ft_isdigit(c));
	// printf("%c is %d\n", c, isdigit(c));
	// return (0);
}
*/