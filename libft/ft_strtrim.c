/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 15:45:17 by analexan          #+#    #+#             */
/*   Updated: 2023/10/09 11:51:07 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countstart(int i, int j, const char *s1, const char *set)
{
	int	start;
	int	setlen;

	start = 0;
	setlen = ft_strlen(set);
	while (s1[i])
	{
		while (j < setlen)
		{
			if (i != start)
				break ;
			else if (s1[i] == set[j])
			{
				start++;
				break ;
			}
			j++;
		}
		j = 0;
		i++;
	}
	return (start);
}

static int	countend(int i, int j, const char *s1, const char *set)
{
	int	end;
	int	setlen;
	int	s1len;

	end = 0;
	setlen = ft_strlen(set);
	s1len = i;
	while (i > 0)
	{
		while (j < setlen)
		{
			if (i != s1len)
				break ;
			else if (s1[i] == set[j])
			{
				s1len--;
				end++;
				break ;
			}
			j++;
		}
		j = 0;
		i--;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		j;
	int		i;
	int		start;
	int		end;

	j = 0;
	i = 0;
	start = countstart(i, j, s1, set);
	i = ft_strlen(s1) - 1;
	end = countend(i, j, s1, set);
	return (ft_substr(s1, start, i + 1 - start - end));
}

/*
// the: if (i != start/s1len) break ;
// is needed to adjust the i so it always (i == start/s1len)
#include <stdio.h>
int	main(void)
{
	char *str = "aababacaaacccb";
 	char *set = "acb";
	int i = 0;

 	// char *ret = ft_strtrim("  \t \t \n   \n\n\n\t", " \n\t");
 	// if (!strcmp(ret, set))
 	// 	printf("yes");
 	// printf("no");
	while (set[i]) 
		i++;
	printf("len(set): %i\n", i);
	printf("set: %s\nbefore: %s\nafter:  %s\n", set, str, ft_strtrim(str, set));
	// set[1] = 0;
	// printf("set: %s\nbefore: %s\nafter:  %s\n", set, str, 
		ft_strtrim(str, set));
	// set[0] = 'i';
	// printf("set: %s\nbefore: %s\nafter:  %s\n", set, str, 
		ft_strtrim(str, set));
	// str[4] = 'y';
	// set[0] = 'h';
	// set[1] = 'i';
	// printf("set: %s\nbefore: %s\nafter:  %s\n", set, str, 
		ft_strtrim(str, set));
	// //free(strs used!);
	// return (0);
}
*/