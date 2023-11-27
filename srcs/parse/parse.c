/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:41:22 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/27 13:14:36 by jealves-         ###   ########.fr       */
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

t_list	*create_word(bool *is_new_cmd)
{
	t_list	*word_p;

	word_p = ft_lstnew(NULL, ft_calloc(sizeof(char *), 1), WORD);
	*is_new_cmd = false;
	return (word_p);
}

void	join_str_word(t_list *word_p, t_list *word_l, int cmd_index)
{
	if (word_p->str == NULL)
		word_p->str = ft_strdup(word_l->str);
	else
	{
		ft_strlcat(word_p->str, " ", ft_strlen(word_p->str) + 2);
		ft_strlcat(word_p->str, word_l->str, ft_strlen(word_p->str)
			+ ft_strlen(word_l->str) + 1);
	}
	if (word_p->type == WORD)
		word_p->type = word_l->type;
	word_p->cmds = (char **)ft_realloc(word_p->cmds, (cmd_index + 2)
			* sizeof(char *));
	word_p->cmds[cmd_index] = ft_strdup(word_l->str);
	word_p->cmds[cmd_index + 1] = NULL;
}

void	add_word_lst(t_list *word_p, bool *is_new_cmd, int *cmd_index)
{
	ft_lstadd_back(&var()->lst_parse, ft_lstnew(word_p->str,
			word_p->cmds, word_p->type));
	free(word_p);
	*is_new_cmd = true;
	*cmd_index = 0;
}
void	print(void)
{
	int		i;
	t_list	*word;

	word = var()->lst_parse;
	while (word)
	{
		i = 0;
		prt("Parser : tipo = %d, palavra = %s\n", word->type, word->str);
		while (word->cmds[i] != NULL)
		{
			prt("Lexer: palavra = %s\n", word->cmds[i]);
			i++;
		}
		word = word->next;
	}
}

void	parse(void)
{
	t_list	*word_l;
	t_list	*word_p;
	bool	is_new_cmd;
	int		cmd_index;

	is_new_cmd = true;
	cmd_index = 0;
	word_l = var()->words;
	while (word_l)
	{
		if (is_new_cmd)
			word_p = create_word(&is_new_cmd);
		if (!ft_strcmpold(word_l->str, "|"))
			join_str_word(word_p, word_l, cmd_index++);
		if (ft_strcmpold(word_l->str, "|") || word_l->next == NULL)
			add_word_lst(word_p, &is_new_cmd, &cmd_index);
		word_l = word_l->next;
	}
	ft_lstclear(&var()->words, free_lst);
	print();
}
