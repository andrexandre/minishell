/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/19 17:21:05 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	search_and_replace(char *str, char src, char dest)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == src)
			str[i] = dest;
		i++;
	}
}

bool	cmd(char *str)
{
	t_word	*word;

	if (ft_strcmpold(str, "cd") || ft_strcmpold(str, "echo") || ft_strcmpold(str, "env")
		|| ft_strcmpold(str, "export") || ft_strcmpold(str, "pwd") || ft_strcmpold(str,
			"unset") || ft_strcmpold(str, "exit"))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = BUILD_IN;
		word->is_builtin = true;
		word->str = ft_strdup(str);
		ft_lstadd_back(&var()->words, ft_lstnew(word));
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	t_word	*word;

	if (ft_strcmpold(str, "|") || (ft_strcmpold(str, "<")) || (ft_strcmpold(str, "<<"))
		|| (ft_strcmpold(str, ">")) || (ft_strcmpold(str, ">>")))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = TOKEN;
		word->is_builtin = false;
		word->str = ft_strdup(str);
		ft_lstadd_back(&var()->words, ft_lstnew(word));
    return (true);
	}
	return (false);
}

void	lexer(char *str)
{
	int		i;
	char	**splitted;
	t_word	*word;

	i = 0;
	search_and_replace(str, '\t', ' ');
	char *trimmed = ft_strtrim(str, " ");
	splitted = ft_split_without(trimmed, ' ',"'\"");
	free(trimmed);
	while (splitted[i])
	{
		if (!cmd(splitted[i]) && !token(splitted[i]))
		{
			word = ft_calloc(sizeof(t_word), 1);
			word->type = WORD;
			word->is_builtin = false;
			word->str = ft_strdup(splitted[i]);
			ft_lstadd_back(&var()->words, ft_lstnew(word));
		}
		i++;
	}
	ft_cleanup_split(splitted, i);
}
