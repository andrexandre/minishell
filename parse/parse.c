/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/10 11:08:49 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*create_word(bool *is_new_cmd)
{
	t_word	*word;

	word = calloc(sizeof(t_word), 1);
	word->type = CMD;
	word->args = NULL;
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
	if (word->args == NULL)
		word->args = ft_lstnewold(src_word);
	else
		ft_lstadd_back(&word->args, ft_lstnewold(src_word));
}

void	add_word_lst(t_word *word, bool *is_new_cmd)
{
	word->str = ft_strtrim(word->str, " ");
	if (var()->lstep_parsed == NULL)
		var()->lstep_parsed = ft_lstnewold(word);
	else
		ft_lstadd_back(&var()->lstep_parsed, ft_lstnewold(word));
	*is_new_cmd = true;
}

void	parse(void)
{
	t_word	*word;
	t_word	*word_p;
	bool	is_new_cmd;
	t_word	*word2;

	is_new_cmd = true;
	while (var()->words)
	{
		word = var()->words->content;
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!word_p->is_builtin)
			word_p->is_builtin = word->is_builtin;
		if (!ft_strcmpold(word->str, "|"))
			join_str_word(word_p, word);
		if (ft_strcmpold(word->str, "|") || var()->words->next == NULL)
			add_word_lst(word_p, &is_new_cmd);
		var()->words = var()->words->next;
	}
	
	while (var()->lstep_parsed)
	{
		word = var()->lstep_parsed->content;
		prt("Parser : tipo = %d, palavra = %s, built-in = %s\n", word->type,
				word->str, word->is_builtin ? "true" : "false");
		while (word->args)
		{
			word2 = word->args->content;
			prt("Lexer: tipo = %d, palavra = %s, built-in = %s\n", word2->type,
					word2->str, word2->is_builtin ? "true" : "false");
			word->args = word->args->next;
		}
		var()->lstep_parsed = var()->lstep_parsed->next;
	}
}
