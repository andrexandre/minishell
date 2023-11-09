/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/08 19:06:55 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd(t_minishell *ms, char *str)
{
	t_word	*word;

	if (ft_strcmp(str, "cd") || ft_strcmp(str, "echo") || ft_strcmp(str, "env")
		|| ft_strcmp(str, "export") || ft_strcmp(str, "pwd") || ft_strcmp(str,
			"unset") || ft_strcmp(str, "exit"))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = BUILD_IN;
		word->is_builin = true;
		word->str = ft_strdup(str);
		if (ms->words == NULL)
			ms->words = ft_lstnew(word);
		else
			ft_lstadd_back(&ms->words, ft_lstnew(word));
		return (true);
	}
	return (false);
}

bool	token(t_minishell *ms, char *str)
{
	t_word	*word;

	if (ft_strcmp(str, "|") || (ft_strcmp(str, ";")) || (ft_strcmp(str, "<"))
		|| (ft_strcmp(str, "<<")) || (ft_strcmp(str, ">")) || (ft_strcmp(str,
				">>")) || (ft_strcmp(str, "&")))
	{
		word = ft_calloc(sizeof(t_word), 1);
		word->type = TOKEN;
		word->is_builin = false;
		word->str = ft_strdup(str);
		if (ms->words == NULL)
			ms->words = ft_lstnew(word);
		else
			ft_lstadd_back(&ms->words, ft_lstnew(word));
		return (true);
	}
	return (false);
}

void	lexer(char *str, t_minishell *ms)
{
	int		i;
	char	**splitted;
	char	*new_str;
	t_word	*word;

	i = 0;
	new_str = ft_substr(str, 0, ft_strlen(str) - 1);
	splitted = ft_split(new_str, ' ');
	while (splitted[i])
	{
		if (!cmd(ms, splitted[i]) && !token(ms, splitted[i]))
		{
			word = ft_calloc(sizeof(t_word), 1);
			word->type = WORD;
			word->is_builin = false;
			word->str = ft_strdup(splitted[i]);
			if (ms->words == NULL)
				ms->words = ft_lstnew(word);
			else
				ft_lstadd_back(&ms->words, ft_lstnew(word));
		}
		i++;
	}
	ft_cleanup_split(splitted, i);
	free(new_str);
}
