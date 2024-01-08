/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2024/01/08 16:28:34 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*ms(void)
{
	static t_var	ms;

	return (&ms);
}

void	prt_strs(char **strs, int n)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		prt("%s", strs[i]);
		if (strs[i + 1])
			prt(" ");
	}
	if (n)
		prt("\n");
}
