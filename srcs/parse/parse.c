/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/22 22:08:47 by jealves-         ###   ########.fr       */
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
	t_word	*word_p;

	word_p = ft_calloc(sizeof(t_word), 1);
	word_p->cmds = ft_calloc(sizeof(char *), 1);
	*is_new_cmd = false;
	return (word_p);
}

void	join_str_word(t_word *word_p, t_word *word_l, int cmd_index)
{
	if (word_p->str == NULL)
		word_p->str = ft_strdup(word_l->str);
	else
	{
		ft_strlcat(word_p->str, " ", ft_strlen(word_p->str) + 2);
		ft_strlcat(word_p->str, word_l->str, ft_strlen(word_p->str)
			+ ft_strlen(word_l->str) + 1);
	}
	if (word_p->type != BUILT_IN)
		word_p->type = word_l->type;
	word_p->cmds = (char **)ft_realloc(word_p->cmds, (cmd_index + 2)
			* sizeof(char *));
	word_p->cmds[cmd_index] = word_l->str;
	word_p->cmds[cmd_index + 1] = NULL;
}

void	add_word_lst(t_word *word_p, bool *is_new_cmd, int *cmd_index)
{
	ft_lstadd_back(&var()->lst_parse, ft_lstnew(word_p));
	*is_new_cmd = true;
	*cmd_index = 0;
}

void	print(void)
{
	int		i;
	t_word	*word;
	t_list	*lst;

	lst = var()->lst_parse;
	while (lst)
	{
		i = 0;
		word = lst->content;
		prt("Parser : tipo = %d, palavra = %s\n", word->type, word->str);
		while (word->cmds[i] != NULL)
		{
			prt("Lexer: palavra = %s\n", word->cmds[i]);
			i++;
		}
		lst = lst->next;
	}
}

void	parse(void)
{
	t_word	*word_l;
	t_word	*word_p;
	t_list	*lst_lexer;
	bool	is_new_cmd;
	int		cmd_index;

	is_new_cmd = true;
	cmd_index = 0;
	lst_lexer = var()->words;
	while (lst_lexer)
	{
		word_l = lst_lexer->content;
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!ft_strcmpold(word_l->str, "|"))
			join_str_word(word_p, word_l, cmd_index++);
		if (ft_strcmpold(word_l->str, "|") || lst_lexer->next == NULL)
			add_word_lst(word_p, &is_new_cmd, &cmd_index);
		lst_lexer = lst_lexer->next;
	}
	ft_lstclear(&var()->words, free_word);
	print();
}
