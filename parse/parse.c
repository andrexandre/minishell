/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/21 19:35:49 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_matrix(char **str)
{
	int	i;

	i = 0;
	while (str != NULL && str[i] != NULL)
		i++;
	return (i);
}

t_word	*create_word(bool *is_new_cmd)
{
	t_word	*word;

	word = ft_calloc(sizeof(t_word), 1);
	word->cmds = ft_calloc(sizeof(char *), 1);
	*is_new_cmd = false;
	return (word);
}

void	join_str_word(t_word *word, t_word *src_word, int cmd_s)
{
	//int		i;
	//i = 0;
	if (word->str == NULL)
		word->str = ft_strdup(src_word->str);
	else
	{
		ft_strlcat(word->str, " ", ft_strlen(word->str) + 2);		
		ft_strlcat(word->str, src_word->str, ft_strlen(word->str) + ft_strlen(src_word->str) + 1);
	}
	if(word->type != BUILT_IN)
		word->type = src_word->type;	
	
	word->cmds = (char **)ft_realloc(word->cmds, (cmd_s + 2) * sizeof(char *));
	word->cmds[cmd_s] = src_word->str;
	word->cmds[cmd_s + 1] = NULL;
}

void	add_word_lst(t_word *word, bool *is_new_cmd, int *cmd_s)
{
	ft_lstadd_back(&var()->lstep_parsed, ft_lstnew(word));
	*is_new_cmd = true;
	*cmd_s = 0;
}

void	parse(void)
{
	t_word	*word;
	t_word	*word_p;
	bool	is_new_cmd;
	t_list	*curr;
	int		cmd_s;

	is_new_cmd = true;
	cmd_s = 0;
	curr = var()->words;
	while (curr)
	{
		word = curr->content;
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!ft_strcmpold(word->str, "|"))
			join_str_word(word_p, word, cmd_s++);
		if (ft_strcmpold(word->str, "|") || curr->next == NULL)
			add_word_lst(word_p, &is_new_cmd, &cmd_s);
		curr = curr->next;
	}
	curr = var()->lstep_parsed;
	while (curr)
	{
		int i = 0;
		word = curr->content;
		prt("Parser : tipo = %d, palavra = %s\n", word->type, word->str);
		while (word->cmds[i] != NULL)
		{
			prt("Lexer: palavra = %s\n", word->cmds[i]);
			i++;
		}
		curr = curr->next;
	}
}
