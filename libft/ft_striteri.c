/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 17:39:45 by analexan          #+#    #+#             */
/*   Updated: 2023/04/24 16:48:59 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = -1;
	while (s[++i])
		f(i, &s[i]);
}
/*
static void	ft_rnd(unsigned int i, char *str)
{
	str[i] += i;
}


#include <stdio.h>
int	main(void)
{
	char	str[] = "howdy";

	printf("str before: %s\n", str);
	ft_striteri(str, ft_rnd);
	printf("str after:  %s\n", str);
	return (0);
}
*/