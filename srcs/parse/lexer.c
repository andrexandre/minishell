/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/11 20:51:49 by jealves-         ###   ########.fr       */
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
				BUILT_IN, false));
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	if (ft_strcmpold(str, "|"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL, PIPE,
				true));
	else if (ft_strcmpold(str, "<"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_IN, true));
	else if (ft_strcmpold(str, "<<"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_IN_D, true));
	else if (ft_strcmpold(str, ">"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_OUT, true));
	else if (ft_strcmpold(str, ">>"))
		ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				REDIRECT_OUT_D, true));
	else
		return (false);
	return (true);
}

char	**split_lexer(char *str)
{
	char	**splitted;
	char	*trimmed;
	char	*str_token;

	str_token = space_token(str);
	search_and_replace(str_token, '\t', ' ');
	trimmed = ft_strtrim(str_token, " ");
	free(str_token);
	if (!trimmed || (trimmed && !*trimmed))
	{
		free(trimmed);
		return (NULL);
	}
	splitted = ft_split_without(trimmed, ' ', "'\"");
	free(trimmed);
	if (splitted == NULL)
	{
		prt("unclosed quote\n");
		return (NULL);
	}
	return (splitted);
}

int	lexer(char *str)
{
	int		i;
	char	**splitted;

	i = 0;
	ms()->origin_str = str;
	splitted = split_lexer(str);
	if (!splitted)
		return (1);
	while (splitted[i])
	{
		if (!builtin(splitted[i]) && !token(splitted[i]))
			ft_lstadd_back(&ms()->lst_lexer, ft_lstnew(ft_strdup(splitted[i]),
					NULL, WORD, false));
		i++;
	}
	free_strs(splitted);
	return (0);
}
