/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_lib.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 17:16:37 by analexan          #+#    #+#             */
/*   Updated: 2023/10/11 16:21:39 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	pn(int num)
{
	if (num < 0)
	{
		write(1, "-", 1);
		if (num == -2147483648)
		{
			write(1, "2", 1);
			num = -147483648;
		}
		num = -num;
	}
	if (num > 9)
		pn(num / 10);
	write(1, &"0123456789"[num % 10], 1);
}

void	ps(char *str, int mode)
{
	if (!str)
	{
		write(1, "(null)", 6);
		return ;
	}
	if (!mode)
		while (*str)
			write(1, str++, 1);
	else
		write(1, &str, 1);
}

// custom printf
void	prt(char *string, ...)
{
	va_list	args;
	int		i;

	va_start(args, string);
	i = 0;
	while (string[i])
	{
		if (string[i] == '%')
		{
			i++;
			if (string[i] == 'i')
				pn(va_arg(args, int));
			else if (string[i] == 's')
				ps(va_arg(args, char *), 0);
			else if (string[i] == 'c')
				ps(va_arg(args, char *), 1);
			i++;
		}
		else
			write(1, &string[i++], 1);
	}
	va_end(args);
}

// returns a section of the string s starting in start only until len characters
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*str;
	unsigned int	i;

	i = 0;
	if (start >= ft_strlen(s) || len == 0)
	{
		str = malloc(1);
		if (!str)
			return (NULL);
		str[0] = 0;
		return (str);
	}
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	while (s[i + start] && i < len)
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = 0;
	return (str);
}

// returns the concatenation of the string s1 with s2
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*str;

	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	i = -1;
	while (s1[++i])
		str[i] = s1[i];
	j = -1;
	while (s2[++j])
		str[i + j] = s2[j];
	str[i + j] = 0;
	return (str);
}
