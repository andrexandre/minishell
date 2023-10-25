/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:56:08 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 13:17:18 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int num)
{
	int	intlen;

	if (num == -2147483648)
		return (10);
	intlen = 0;
	if (num < 1)
		intlen++;
	while (num)
	{
		intlen++;
		num /= 10;
	}
	return (intlen);
}

static void	ft_changestr(int n, int c, char *str, int mc)
{
	str[c] = 0;
	if (n == 0)
		str[0] = '0';
	if (mc == -1)
		str[0] = '-';
	if (n == -2147483648)
	{
		c--;
		n /= 10;
		n = -n;
		str[0] = '-';
		str[c] = '8';
	}
	while (n > 0)
	{
		str[c-- - 1] = (n % 10) + '0';
		n /= 10;
	}
}

char	*ft_itoa(int n)
{
	int			mc;
	int			c;
	char		*str;

	mc = 1;
	c = 1;
	if (n < 0)
	{
		mc = -1;
		if (n != -2147483648)
			n = -n;
		c++;
	}
	c += ft_intlen(n);
	str = malloc(c--);
	if (!str)
		return (NULL);
	ft_changestr(n, c, str, mc);
	return (str);
}
/*
#include <stdio.h>
int	main(void)
{
	int	a = +123;
	int	b = -123;
	int	c = 0;
	int d = 2147483647;
	int e = -2147483648;

	printf("inta: %i intb: %i intc: %i intd: %i inte: %i\n", a, b, c, d, e);
	printf("ft_a: %s ft_b: %s ft_c: %s ft_d: %s ft_e: %s\n", ft_itoa(a), 
	ft_itoa(b), ft_itoa(c), ft_itoa(d), ft_itoa(e));
	return (0);
}
*/
