/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:25:51 by analexan          #+#    #+#             */
/*   Updated: 2023/04/25 10:31:33 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = -1;
	if (!s || fd < 0)
		return ;
	while (s[++i])
		write(fd, &s[i], 1);
	write(fd, "\n", 1);
}

/*
int	main(void)
{
	ft_putendl_fd("Hi!", 1);
	return (0);
}
*/