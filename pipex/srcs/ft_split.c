/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:05:24 by analexan          #+#    #+#             */
/*   Updated: 2023/10/19 16:55:43 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	*free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return (NULL);
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (NULL);
}

char	*ft_strndup(char const *src, int n)
{
	int		i;
	char	*str;

	i = -1;
	str = malloc(n + 1);
	if (!str)
		return (NULL);
	while (src[++i] && i < n)
		str[i] = src[i];
	str[i] = 0;
	return (str);
}

int	wordcount(char const *s, char c)
{
	int	i;
	int	wc;

	i = -1;
	wc = 0;
	while (s && s[++i])
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			wc++;
	return (wc);
}

char	**writestring(char const *s, char **strs, char c, int wc)
{
	int	i;
	int	j;

	i = 0;
	while (i < wc)
	{
		j = 0;
		while (*s == c)
			s++;
		while (s[j] && s[j] != c)
			j++;
		strs[i] = ft_strndup(s, j);
		if (!strs[i])
			return (free_strs(strs));
		s += j;
		i++;
	}
	strs[i] = NULL;
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	int		wc;

	wc = wordcount(s, c);
	if (wc < 0)
		return (NULL);
	strs = malloc((wc + 1) * sizeof(char *));
	if (!strs || !s)
		return (NULL);
	strs = writestring(s, strs, c, wc);
	return (strs);
}
