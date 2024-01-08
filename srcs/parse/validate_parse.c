/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 20:09:45 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/08 16:08:07 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_get_last_pipe(void)
{
	t_list	*last;

	last = ft_lstlast(ms()->lst_lexer);
	while (last)
	{
		if (last->type == PIPE)
			break ;
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
			dprt(2,"%s\n","syntax error near unexpected token `|'");
			return (false);
		}
		curr = curr->next;
	}
	return (true);
}

bool	validate_pipe(void)
{
	t_list	*last;

	last = ft_get_last_pipe();
	if (ms()->lst_lexer && ms()->lst_lexer->type == PIPE)
	{
		dprt(2,"%s\n","syntax error near unexpected token `|'");
		return (false);
	}
	if (last && last->type == PIPE)
	{
		if (last->prev->prev && last->prev->prev->prev
			&& last->prev->type == PIPE && last->prev->prev->type == PIPE
			&& last->prev->prev->prev->type == PIPE)
			dprt(2,"%s\n","syntax error near unexpected token `||'");
		else if (last->prev->type == PIPE || last->type == PIPE)
			dprt(2,"%s\n","syntax error near unexpected token `|'");
		return (false);
	}
	if (!validate_curr_pipe())
		return (false);
	return (true);
}

bool	validate_newline(void)
{
	t_list	*last;

	last = ft_lstlast(ms()->lst_lexer);
	if (last->prev && last->type == PIPE && last->prev->type == REDIRECT_OUT)
	{
		dprt(2,"%s\n","syntax error near unexpected token `newline'");
		return (false);
	}
	if (last->prev && last->type == REDIRECT_OUT && last->prev->type == PIPE)
	{
		dprt(2,"%s\n","syntax error near unexpected token `newline'");
		return (false);
	}
	return (true);
}

bool	validate_parse(void)
{
	t_list	*last;

	last = ft_lstlast(ms()->lst_lexer);
	if (!validate_newline())
		return (false);
	if (!validate_pipe())
		return (false);
	if (last->type == REDIRECT_IN || last->type == REDIRECT_IN_D
		|| last->type == REDIRECT_OUT || last->type == REDIRECT_OUT_D)
	{
		dprt(2,"%s %s\n","syntax error near unexpected token ",last->str);
		return (false);
	}
	return (true);
}
