/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/14 22:11:30 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*create_word(bool *is_new_cmd)
{
	t_word	*word;

	word = ft_calloc(sizeof(t_word), 1);
	word->type = CMD;
	*is_new_cmd = false;
	word->is_builtin = false;
	return (word);
}

void	join_str_word(t_word *word, t_word *src_word)
{
	char	*cmd;

	if (word->str == NULL)
		word->str = ft_strdup(src_word->str);
	else
	{
		cmd = ft_strjoin(word->str, " ");
		free(word->str);
		word->str = ft_strjoin(cmd, src_word->str);
		free(cmd);
	}
}

void	add_word_lst(t_word *word, bool *is_new_cmd)
{
	char *temp = word->str;
	word->str = ft_strtrim(temp, " ");
	word->cmds = ft_split(word->str, ' ');
	free(temp);
	ft_lstadd_back(&var()->lstep_parsed, ft_lstnew(word));
	*is_new_cmd = true;
}

void	parse(void)
{
	t_word	*word;
	t_word	*word_p;
	bool	is_new_cmd;
	t_list	*curr;

	is_new_cmd = true;
	curr = var()->words;
	while (curr)
	{
		word = curr->content;
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!word_p->is_builtin)
			word_p->is_builtin = word->is_builtin;
		if (!ft_strcmpold(word->str, "|"))
			join_str_word(word_p, word);
		if (ft_strcmpold(word->str, "|") || curr->next == NULL)
			add_word_lst(word_p, &is_new_cmd);
		curr = curr->next;
	}
	curr = var()->lstep_parsed;
	while (curr)
	{
		int i = 0;
		word = curr->content;
		prt("Parser : tipo = %d, palavra = %s, built-in = %s\n", word->type,
				word->str, word->is_builtin ? "true" : "false");
		while (word->cmds[i])
		{
			prt("Lexer: palavra = %s\n", word->cmds[i]);
			i++;
		}
		curr = curr->next;
	}
}
