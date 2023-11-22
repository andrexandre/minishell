/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/22 22:08:33 by jealves-         ###   ########.fr       */
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

void	init(char *str, enum e_type type)
{
	t_word	*word;

	word = ft_calloc(sizeof(t_word), 1);
	word->type = type;
	word->str = ft_strdup(str);
	ft_lstadd_back(&var()->words, ft_lstnew(word));
}

bool	cmd(char *str)
{
	if (ft_strcmpold(str, "cd") || ft_strcmpold(str, "echo")
		|| ft_strcmpold(str, "env") || ft_strcmpold(str, "export")
		|| ft_strcmpold(str, "pwd") || ft_strcmpold(str, "unset")
		|| ft_strcmpold(str, "exit"))
	{
		init(str, BUILT_IN);
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	if (ft_strcmpold(str, "|") || (ft_strcmpold(str, "<")) || (ft_strcmpold(str,
				"<<")) || (ft_strcmpold(str, ">")) || (ft_strcmpold(str, ">>")))
	{
		init(str, TOKEN);
		return (true);
	}
	return (false);
}

void	lexer(char *str)
{
	int		i;
	char	**splitted;
	char	*trimmed;

	i = 0;
	search_and_replace(str, '\t', ' ');
	trimmed = ft_strtrim(str, " ");
	splitted = ft_split_without(trimmed, ' ', "'\"");
	free(trimmed);
	while (splitted[i])
	{
		if (!cmd(splitted[i]) && !token(splitted[i]))
			init(splitted[i], WORD);
		i++;
	}
	free_strs(splitted);
}
