/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/09 15:11:46 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*create_word(bool *is_new_cmd)
{
	t_word	*word;
	word = calloc(sizeof(t_word), 1);
	word->type = CMD;
	*is_new_cmd = false;
	return (word);
}

void	join_str_word(t_word *word, char *str)
{
	char	*cmd;

	if (word->str == NULL)
		word->str = ft_strdup(str);
	else
	{
		cmd = ft_strjoin(word->str, " ");
		free(word->str);
		word->str = ft_strjoin(cmd, str);
		free(cmd);
	}
}

void	add_word_lst(t_word *word, bool *is_new_cmd,
		bool *is_builtin)
{
	word->is_builtin = is_builtin;
	word->str = ft_strtrim(word->str, " ");
	if (var()->lstep_parsed == NULL)
		var()->lstep_parsed = ft_lstnewold(word);
	else
		ft_lstadd_back(&var()->lstep_parsed, ft_lstnewold(word));
	*is_new_cmd = true;
	*is_builtin = false;
}

void	parse()
{
	t_word	*word;
	t_word	*word_p;
	bool	is_builtin;
	bool	is_new_cmd;

	is_builtin = false;
	is_new_cmd = true;
	while (var()->words)
	{
		word = var()->words->content;
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!is_builtin)
			is_builtin = word->is_builtin;
		if (!ft_strcmp(word->str, "|"))
			join_str_word(word_p, word->str);
		if (ft_strcmp(word->str, "|") || var()->words->next == NULL)
			add_word_lst(word_p,  &is_new_cmd, &is_builtin);
		var()->words = var()->words->next;
	}
	while (var()->lstep_parsed)
	{
		word = var()->lstep_parsed->content;
		prt("tipo = %d, palavra = %s, buildin = %s\n", word->type, word->str,
				word->is_builtin ? "true" : "false");
		var()->lstep_parsed = var()->lstep_parsed->next;
	}
}
