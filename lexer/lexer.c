/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/06 18:18:58 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd(t_words *words, char *str)
{
	if (ft_strcmp(str, "cd") || ft_strcmp(str, "echo") || ft_strcmp(str, "env")
		|| ft_strcmp(str, "export") || ft_strcmp(str, "pwd") || ft_strcmp(str,
			"unset") || ft_strcmp(str, "exit"))
	{
		words->token = CMD;
		words->word = str;
		return (true);
	}
	return (false);
}

bool	token(t_words *words, char *str)
{
	if (ft_strcmp(str, "|"))
		words->token = PIPE;
	if (ft_strcmp(str, ";"))
		words->token = SEMICOLON;
	if (ft_strcmp(str, "<") || ft_strcmp(str, "<<"))
		words->token = REDIRECT_IN;
	if (ft_strcmp(str, ">") || ft_strcmp(str, ">>"))
		words->token = REDIRECT_OUT;
	if (ft_strcmp(str, "&"))
		words->token = AND;
	if ((PIPE == words->token) || (SEMICOLON == words->token)
		|| (REDIRECT_IN == words->token) || (REDIRECT_OUT == words->token)
		|| (AND == words->token))
	{
		words->word = str;
		return (true);
	}
	return (false);
}

void	lexer(char *str)
{
	int		i;
	char	**splitted;
	t_words	*words;

	i = 0;
	splitted = ft_split(str, ' ');
	words = ft_calloc(sizeof(t_words), ft_strlen_matrix(splitted));
	while (splitted[i])
	{
		if (!cmd(&words[i], splitted[i]) && !token(&words[i], splitted[i]))
		{
			words[i].token = WORD;
			words[i].word = splitted[i];
		}
		i++;
	}
	i = 0;
	while (i < (int)ft_strlen_matrix(splitted))
	{
		prt("tipo = %d, palavra = %s\n", words[i].token, words[i].word);
		i++;
	}
}
