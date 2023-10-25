/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_lib2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 17:16:37 by analexan          #+#    #+#             */
/*   Updated: 2023/10/12 17:42:16 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

size_t	ft_strlen(char const *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

// searches in the string big for little and returns the a pointer to 
// the first character of little in the string big else returns NULL
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	int				j;
	char			*bptr;
	char			*lptr;

	bptr = (char *)big;
	lptr = (char *)little;
	i = 0;
	j = 0;
	if (!lptr[i])
		return (bptr);
	while (bptr[i])
	{
		while (bptr[i + j] && bptr[i + j] == lptr[j] && i + j < len)
			if (!lptr[j++ + 1])
				return (&bptr[i]);
		j = 0;
		i++;
	}
	return (0);
}

// this function is a global var without norm error
t_vars	*vars(void)
{
	static t_vars	vars;

	return (&vars);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned int	i;
	char			*ptr;
	void			*arr;

	arr = malloc(nmemb * size);
	if (!arr)
		return (NULL);
	i = 0;
	ptr = (char *)arr;
	while (i < nmemb * size)
	{
		ptr[i] = 0;
		i++;
	}
	return (arr);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (0);
}
