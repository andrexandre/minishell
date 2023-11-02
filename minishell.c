/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/02 13:52:08 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
// int main(int ac, char **av, char **ep)
{
	char	*buf;

	prt("> ");
	buf = get_next_line(0);
	while (buf)
	{
		free(buf);
		prt("> ");
		buf = get_next_line(0);
		if (!ft_strncmp(buf, "exit", 4) || !ft_strncmp(buf, "q", 1))
			break ;
	}
	free(buf);
	prt("\n");
	return (0);
}
