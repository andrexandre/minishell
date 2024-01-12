/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirect_parse.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 20:26:46 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/12 23:06:22 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_get_last_type(int type)
{
	t_list	*last;

	last = ft_lstlast(ms()->lst_lexer);
	while (last)
	{
		if (type == PIPE)
		{
			if (last->type == PIPE)
				break ;
		}
		else
		{
			if (last->type == REDIRECT_IN || last->type == REDIRECT_IN_D
				|| last->type == REDIRECT_OUT || last->type == REDIRECT_OUT_D)
				break ;
		}
		last = last->prev;
	}
	return (last);
}

bool	unexpected_redirect(t_list *last)
{
	int	i;

	if (last && last->type == REDIRECT_OUT)
	{
		i = ft_strlen(ms()->origin_str);
		while (--i >= 0)
		{
			if (ms()->origin_str[i] == '>' && i > 2
				&& ms()->origin_str[i - 2] == '<')
			{
				dprt(2, "minishell: syntax error near unexpected token `>'\n");
				return (false);
			}
		}
	}
	if (last && last->prev && last->prev->type != PIPE && (last->prev->token))
	{
		dprt(2, "minishell: syntax error near unexpected token `%s'\n",
			last->str);
		return (false);
	}
	return (true);
}

bool	newline_redirect(t_list *last)
{
	if (last && !last->next)
	{
		dprt(2, "minishell: syntax error near unexpected token `newline'\n");
		return (false);
	}
	if (last && !last->next && last->prev && last->prev->type == PIPE)
	{
		dprt(2, "minishell: syntax error near unexpected token `newline'\n");
		return (false);
	}
	return (true);
}

bool	validate_redirect(void)
{
	t_list	*last;

	last = ft_get_last_type(REDIRECT_IN);
	if (!unexpected_redirect(last) || !newline_redirect(last))
		return (false);
	return (true);
}
