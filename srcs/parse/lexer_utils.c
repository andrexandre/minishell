/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/12 21:17:10 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_scnd_token(char *str, int i, int j)
{
	if (str[i] == '|')
		return (false);
	if (str[i] == str[j] && (j - i) < 2 && (str[j] == '<' || str[j] == '>'))
		return (true);
	return (false);
}

bool	is_token(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	return (false);
}

int	ft_put_space(int i, char **result)
{
	char	*space;
	int		j;
	int		len;
	char	*res;

	res = *result;
	len = ft_strlen(*result);
	j = i + 1;
	while (len > j && res[j])
	{
		if (!is_scnd_token(res, i, j))
			break ;
		j++;
	}
	space = ft_strdup(" ");
	ft_strrep(result, j, j, space);
	ft_strrep(result, i, i, space);
	free(space);
	return (j);
}

char	*space_token(char *str)
{
	int		i;
	char	*result;
	bool	quote;
	bool	dquote;

	i = -1;
	quote = false;
	dquote = false;
	result = ft_strdup(str);
	while (result[++i])
	{
		if (result[i] == '\'')
			quote = !quote;
		if (result[i] == '"')
			dquote = !dquote;
		if (is_token(result[i]) && !quote && !dquote)
		{
			i = ft_put_space(i, &result);
		}
	}
	return (result);
}

int	count_to_pipe(t_list *words)
{
	int		i;
	t_list	*cur;

	i = 1;
	cur = words;
	while (cur && cur->type != PIPE)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}
