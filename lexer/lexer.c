/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/09 15:05:37 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd(char *str)
{
	t_word	*word;

	if (ft_strcmp(str, "cd") || ft_strcmp(str, "echo") || ft_strcmp(str, "env")
		|| ft_strcmp(str, "export") || ft_strcmp(str, "pwd") || ft_strcmp(str,
			"unset") || ft_strcmp(str, "exit"))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = BUILD_IN;
		word->is_builtin = true;
		word->str = ft_strdup(str);
		if (var()->words == NULL)
			var()->words = ft_lstnewold(word);
		else
			ft_lstadd_back(&var()->words, ft_lstnewold(word));
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	t_word	*word;

	if (ft_strcmp(str, "|") || (ft_strcmp(str, ";")) || (ft_strcmp(str, "<"))
		|| (ft_strcmp(str, "<<")) || (ft_strcmp(str, ">")) || (ft_strcmp(str,
				">>")) || (ft_strcmp(str, "&")))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = TOKEN;
		word->is_builtin = false;
		word->str = ft_strdup(str);
		if (var()->words == NULL)
			var()->words = ft_lstnewold(word);
		else
			ft_lstadd_back(&var()->words, ft_lstnewold(word));
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
	splitted = ft_split(ft_strtrim(str, " "), ' ');
	while (splitted[i])
	{
		if (!cmd(splitted[i]) && !token(splitted[i]))
		{
			word = ft_calloc(sizeof(t_word), 1);
			word->type = WORD;
			word->is_builtin = false;
			word->str = ft_strdup(splitted[i]);
			if (var()->words == NULL)
				var()->words = ft_lstnewold(word);
			else
				ft_lstadd_back(&var()->words, ft_lstnewold(word));
		}
		i++;
	}
	ft_cleanup_split(splitted, i);
}
