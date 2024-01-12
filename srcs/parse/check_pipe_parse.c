/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 20:09:45 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/12 11:02:10 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_curr_pipe(void)
{
	t_list	*curr;

	curr = ms()->lst_lexer;
	while (curr && curr->next)
	{
		if (curr->type == PIPE && curr->next->type == PIPE)
		{
			dprt(2, "minishell: syntax error near unexpected token `|'\n");
			return (false);
		}
		curr = curr->next;
	}
	return (true);
}

bool	unexpected_pipe(t_list *last)
{
	if (last && last->type == PIPE)
	{
		if (last->prev->prev && last->prev->prev->prev
			&& last->prev->type == PIPE && last->prev->prev->type == PIPE
			&& last->prev->prev->prev->type == PIPE)
		{
			dprt(2, "minishell: syntax error near unexpected token `||'\n");
			return (false);
		}
		else if (!last->next && (last->prev->type == PIPE
				|| last->type == PIPE))
		{
			dprt(2, "minishell: syntax error near unexpected token `|'\n");
			return (false);
		}
		else if (last->prev->token && last->prev->type != PIPE)
		{
			dprt(2, "minishell: syntax error near unexpected token `|'\n");
			return (false);
		}
	}
	return (true);
}

bool	validate_pipe(void)
{
	t_list	*last;

	last = ft_get_last_type(PIPE);
	if (ms()->lst_lexer && ms()->lst_lexer->type == PIPE)
	{
		dprt(2, "minishell: syntax error near unexpected token `|'\n");
		return (false);
	}
	if (!unexpected_pipe(last) || !validate_curr_pipe())
		return (false);
	return (true);
}

bool	validate_parse(void)
{
	if (!validate_redirect() || !validate_pipe())
	{
		ms()->status = 2;
		return (false);
	}
	return (true);
}
