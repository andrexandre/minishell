/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/28 14:59:02 by analexan         ###   ########.fr       */
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
	if (ft_strcmpold(str, "cd") || ft_strcmpold(str, "echo")
		|| ft_strcmpold(str, "env") || ft_strcmpold(str, "export")
		|| ft_strcmpold(str, "pwd") || ft_strcmpold(str, "unset")
		|| ft_strcmpold(str, "exit"))
	{
		ft_lstadd_back(&var()->lst_lexer, ft_lstnew(ft_strdup(str), NULL,
				BUILT_IN));
		return (true);
	}
	return (false);
}

bool	token(char *str)
{
	if (ft_strcmpold(str, "|") || (ft_strcmpold(str, "<")) || (ft_strcmpold(str,
				"<<")) || (ft_strcmpold(str, ">")) || (ft_strcmpold(str, ">>")))
	{
		ft_lstadd_back(&var()->lst_lexer, ft_lstnew(ft_strdup(str), NULL, TOKEN));
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
			ft_lstadd_back(&var()->lst_lexer, ft_lstnew(ft_strdup(splitted[i]),
					NULL, WORD));
		i++;
	}
	free_strs(splitted);
}
