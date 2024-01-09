/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2024/01/09 15:08:38 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**remove_items(char **strs, int n)
{
	char	**new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (strs[i])
		i++;
	if (n < 0 || n >= i - 1)
		return (strs);
	new = ft_calloc(i - 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		if (i != n && i != n + 1)
			new[j++] = ft_strdup(strs[i]);
		i++;
	}
	new[j] = NULL;
	free_strs(strs);
	return (new);
}

int	fd_error(t_list *curr, int *i, int *error)
{
	if (ms()->fd[0] < 0 || ms()->fd[1] < 0)
	{
		if (ms()->status == 130)
			*error = 2;
		else
		{
			*error = 1;
			dprt(2, "minishell: ");
			perror(curr->cmds[*i + 1]);
			ms()->status = 1;
		}
		if (ms()->fd[0] < 0)
			ms()->fd[0] = 0;
		else if (ms()->fd[1] < 0)
			ms()->fd[1] = 0;
		return (1);
	}
	if (ft_strlen_matrix(curr->cmds) < 3)
		*error = 1;
	curr->cmds = remove_items(curr->cmds, *i);
	*i = *i - 1;
	return (0);
}

// NOT TOO MANY LINES
void	redirects(t_list *curr, int *error)
{
	int	i;

	i = -1;
	while (curr->cmds[++i])
	{
		if ((!ft_strcmp(curr->cmds[i], "<") || !ft_strcmp(curr->cmds[i], "<<")
				|| !ft_strcmp(curr->cmds[i], ">")
				|| !ft_strcmp(curr->cmds[i], ">>")) && !curr->cmds[i + 1])
		{
			dprt(2, "minishell: syntax error near unexpected token `newline'\n");
			dprt(2, "chegou ao executor :(\n");
			*error = 2;
			ms()->status = 2;
			close_pipes(-1);
			break ;
		}
		if (ms()->fd[0] && curr->cmds[i][0] == '<')
			close(ms()->fd[0]);
		if (ms()->fd[1] && curr->cmds[i][0] == '>')
			close(ms()->fd[1]);
		if (!ft_strcmp(curr->cmds[i], "<"))
			(ms()->fd[0]) = open(curr->cmds[i + 1], O_RDONLY);
		else if (!ft_strcmp(curr->cmds[i], "<<"))
			(ms()->fd[0]) = heredoc(curr->cmds[i + 1]);
		else if (!ft_strcmp(curr->cmds[i], ">"))
			(ms()->fd[1]) = open(curr->cmds[i + 1],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (!ft_strcmp(curr->cmds[i], ">>"))
			(ms()->fd[1]) = open(curr->cmds[i + 1],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			continue ;
		if (fd_error(curr, &i, error))
			break ;
	}
}

int	_is_builtin(char *str)
{
	if (!ft_strcmp(str, "cd") || !ft_strcmp(str, "echo")
		|| !ft_strcmp(str, "env") || !ft_strcmp(str, "export")
		|| !ft_strcmp(str, "pwd") || !ft_strcmp(str, "unset")
		|| !ft_strcmp(str, "exit"))
		return (1);
	return (0);
}
