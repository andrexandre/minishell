/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_without.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 16:58:53 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/22 17:19:19 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


bool	ft_ignored_lst(bool *ignored, char *ignored_lst)
{
	int	i;

	i = 0;
	while (ft_strlen(ignored_lst) > (size_t)i)
	{
		if (ignored[i])
			return (true);
		i++;
	}
	return (false);
}

bool	*init_ignored(char *ignored_lst)
{
	int		i;
	bool	*ignored;

	i = 0;
	ignored = ft_calloc(ft_strlen(ignored_lst), 1);
	while (ignored_lst[i])
	{
		ignored[i] = false;
		i++;
	}
	return (ignored);
}

static int	count_words_without(const char *str, char c, char *ignored_lst)
{
	int		i;
	int		j;
	int		trigger;
	bool	*ignored;

	i = 0;
	j = 0;
	trigger = 0;
	ignored = init_ignored(ignored_lst);
	while (*str)
	{
		if (*str != c && trigger == 0 && !ft_ignored_lst(ignored, ignored_lst))
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		j = 0;
		
		while (ignored_lst[j])
			if (*str == ignored_lst[j++])
				ignored[j - 1] = !ignored[j - 1];
		str++;
	}
	return (i);
}

char	**ft_write_words_without(char **split, char const *s, char c,
		char *ignored_lst)
{
	size_t	i;
	size_t	j;
	size_t	k;
	int		start;
	bool	*ignored;

	i = 0;
	j = 0;
	k = 0;
	start = -1;
	ignored = init_ignored(ignored_lst);
	while (i <= ft_strlen(s))
	{
		k = 0;
		while (ignored_lst[k])
			if (s[i] == ignored_lst[k++])
				ignored[k - 1] = !ignored[k - 1];
		if (s[i] != c && start < 0)
			start = i;
		else if ((s[i] == c || i == ft_strlen(s)) && start >= 0
			&& !ft_ignored_lst(ignored, ignored_lst))
		{
			split[j] = ft_calloc((i - start + 1), sizeof(char));
			if (!split[j])
				return (ft_cleanup_split(split, j));
			ft_strlcpy(split[j++], s + start, i - start + 1);
			start = -1;
		}
		i++;
	}
	free(ignored);
	split[j] = 0;
	return (split);
}

char	**ft_split_without(char const *s, char c, char *ignored_lst)
{
	char	**split;

	split = ft_calloc((count_words_without(s, c, ignored_lst) + 1),
			sizeof(char *));
	if (!s || !split)
		return (0);
	return (ft_write_words_without(split, s, c, ignored_lst));
}
