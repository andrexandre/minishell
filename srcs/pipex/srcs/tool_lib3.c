/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_lib3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 17:16:37 by analexan          #+#    #+#             */
/*   Updated: 2023/11/13 12:18:39 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		i;

	i = 0;
	while (s[i])
		i++;
	str = ft_calloc(i + 1, 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void	free_all(int n)
{
	int	i;

	free_strs(var()->paths);
	i = -1;
	while (++i < var()->ac - 3 - var()->hd)
		free_strs(var()->cmdargs[i]);
	free(var()->cmdargs);
	i = -1;
	while (++i < var()->ac - 4)
		free(var()->pipe[i]);
	free(var()->pipe);
	if (n)
		return ;
	free(var()->pids);
}

void	close_all(int fd1, int fd2)
{
	int		i;

	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
	i = -1;
	while (++i < var()->ac - 4)
	{
		close(var()->pipe[i][0]);
		close(var()->pipe[i][1]);
	}
}

void	error_b(int n)
{
	if (n == -1)
		ft_printf("usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
	else if (!n)
		ft_printf("usage: ./pipex <file1> <cmd1> <cmd2> ... <file2>\n");
	else if (n == 1)
		free_all(1);
	else if (n == 4)
		ft_printf("usage: ./pipex here_doc EOF <cmd1> <cmd2> <file>\n");
	else
		free_all(0);
	if (n == 3)
	{
		if (!var()->av[1] || !*var()->av[1])
			perror("''");
		else
			perror(var()->av[1]);
	}
	else if (n > 1 && n != 4)
		perror("pipex");
	exit(EXIT_FAILURE);
}
