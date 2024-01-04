/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/04 22:39:41 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	builtin(char *str)
{
	if (ft_strcmpold(str, "cd") || ft_strcmpold(str, "echo")
		|| ft_strcmpold(str, "env") || ft_strcmpold(str, "export")
		|| ft_strcmpold(str, "pwd") || ft_strcmpold(str, "unset")
		|| ft_strcmpold(str, "exit"))
	{
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				BUILT_IN));
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	if (ft_strcmpold(str, "|"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				PIPE));
	else if (ft_strcmpold(str, "<"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_IN));
	else if (ft_strcmpold(str, "<<"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_IN_D));
	else if (ft_strcmpold(str, ">"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_OUT));
	else if (ft_strcmpold(str, ">>"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_OUT_D));
	else
		return (false);
	return (true);
}

void	word(char *str)
{
	ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(expander(ft_strdup(str)), NULL, WORD));
}

int	lexer(char *str)
{
	int		i;
	char	**splitted;
	char	*trimmed;

	i = 0;
	space_token(str);
	search_and_replace(str, '\t', ' ');
	trimmed = ft_strtrim(str, " ");
	if (!trimmed || (trimmed && !*trimmed))
	{
		free(trimmed);
		return (1);
	}
	splitted = ft_split_without(trimmed, ' ', "'\"");
	free(trimmed);
	if (splitted == NULL)
	{
		prt("unclosed quote\n");
		return (1);
	}
	while (splitted[i])
	{
		if (!builtin(splitted[i]) && !token(splitted[i]))
			word(splitted[i]);
		i++;
	}
	free_strs(splitted);
	return (0);
}
