/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 20:09:45 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/09 16:12:33 by jealves-         ###   ########.fr       */
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

bool	validate_curr_pipe(void)
{
	t_list	*curr;

	curr = ms()->lst_lexer;
	while (curr && curr->next)
	{
		if (curr->type == PIPE && curr->next->type == PIPE)
		{
			dprt(2, "%s\n",
				"minishell: syntax error near unexpected token `|'");
			return (false);
		}
		curr = curr->next;
	}
	return (true);
}

bool	validate_pipe(void)
{
	t_list	*last;

	last = ft_get_last_type(PIPE);
	if (ms()->lst_lexer && ms()->lst_lexer->type == PIPE)
	{
		dprt(2, "%s\n", "minishell: syntax error near unexpected token `|'");
		return (false);
	}
	if (last && last->type == PIPE)
	{
		if (last->prev->prev && last->prev->prev->prev
			&& last->prev->type == PIPE && last->prev->prev->type == PIPE
			&& last->prev->prev->prev->type == PIPE)
			{
			dprt(2, "%s\n",
				"minishell: syntax error near unexpected token `||'");
				return(false);
			}
		else if (!last->next && (last->prev->type == PIPE || last->type == PIPE))
		{
			dprt(2, "%s\n",
				"minishell: syntax error near unexpected token `|'");
			return (false);			
		}
		else if (last->prev->token && last->prev->type != PIPE )
		{
			dprt(2, "%s\n",
				"minishell: syntax error near unexpected token `|'");
			return (false);			
		}
	}
	if (!validate_curr_pipe())
		return (false);
	return (true);
}

bool	validate_newline(void)
{
	t_list	*last;

	last = ft_get_last_type(REDIRECT_IN);
	if (last && !last->next && (last->type == REDIRECT_IN
			|| last->type == REDIRECT_IN_D || last->type == REDIRECT_OUT
			|| last->type == REDIRECT_OUT_D))
	{
		dprt(2, "%s\n",
			"minishell: syntax error near unexpected token `newline'");
		return (false);
	}
	if (last && !last->next && last->prev && last->prev->type == PIPE
		&& (last->type == REDIRECT_OUT || last->type == REDIRECT_OUT_D
			|| last->type == REDIRECT_IN || last->type == REDIRECT_IN_D))
	{
		dprt(2, "%s\n",
			"minishell: syntax error near unexpected token `newline'");
		return (false);
	}
	return (true);
}

bool	validate_parse(void)
{
	t_list	*last;

	last = ft_get_last_type(REDIRECT_IN);
	if (last && last->prev && (last->type == REDIRECT_IN
			|| last->type == REDIRECT_IN_D) && (last->prev->type == REDIRECT_OUT
			|| last->prev->type == REDIRECT_OUT_D
			|| last->prev->type == REDIRECT_IN
			|| last->prev->type == REDIRECT_IN_D))
	{
		dprt(2, "%s %s\n", "minishell: syntax error near unexpected token",
			last->str);
		ms()->status = 2;
		return (false);
	}
	if (!validate_newline())
	{
		ms()->status = 2;
		return (false);
	}
	if (!validate_pipe())
	{
		ms()->status = 2;
		return (false);
	}
	return (true);
}
