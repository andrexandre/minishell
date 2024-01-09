/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2024/01/09 15:54:16 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ep_from_epl(void)
{
	t_eplist	*curr;
	char		**ep;
	int			i;

	curr = ms()->epl;
	i = 0;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	ep = ft_calloc(i + 1, sizeof(char *));
	if (!ep)
		return (NULL);
	i = -1;
	curr = ms()->epl;
	while (curr)
	{
		if (ft_strchr(curr->str, '='))
			ep[++i] = ft_strdup(curr->str);
		curr = curr->next;
	}
	ep[i + 1] = NULL;
	return (ep);
}

int	has_slash(char *command)
{
	struct stat	statbuf;

	if (stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		dprt(2, "minishell: %s: is a directory\n", command);
	else if (access(command, F_OK | X_OK | R_OK))
	{
		dprt(2, "minishell: ");
		perror(command);
	}
	else
		return (1);
	if (access(command, F_OK))
		ms()->status = 127;
	else
		ms()->status = 126;
	return (0);
}

char	*search_cmd(char *command, char **ep)
{
	int			i;
	char		*cmd;
	struct stat	statbuf;

	ms()->status = 127;
	if (!*command)
		dprt(2, "'': command not found\n");
	else if (!ft_strchr(command, '/') && get_env("PATH") && ms()->paths)
	{
		i = -1;
		while (ms()->paths[++i])
		{
			cmd = ft_strjoin(ms()->paths[i], command);
			if (!access(cmd, F_OK | X_OK)
				&& !(stat(command, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)))
				return (cmd);
			free(cmd);
		}
		dprt(2, "%s: command not found\n", command);
	}
	else if (has_slash(command))
		return (ft_strdup(command));
	free_strs(ep);
	free_all(ms()->status, 0);
	return (NULL);
}

void	parsing_paths(void)
{
	char	*temp;
	int		i;

	if (!get_env("PATH"))
		return ;
	free_strs(ms()->paths);
	(ms()->paths) = ft_split(get_env("PATH")->data, ':');
	if (!ms()->paths)
		return ;
	i = -1;
	while (ms()->paths[++i])
	{
		temp = ms()->paths[i];
		(ms()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!ms()->paths[i])
			return ;
	}
}

void	run_child(char **ep, t_list *curr)
{
	char	*cmd;

	cmd = NULL;
	parsing_paths();
	if (curr->type != BUILT_IN)
		cmd = search_cmd(curr->cmds[0], ep);
	if (ms()->fd[0] && dup2(ms()->fd[0], STDIN_FILENO) < 0)
		perror("dup2, fd[0]");
	if (ms()->fd[1] && dup2(ms()->fd[1], STDOUT_FILENO) < 0)
		perror("dup2, fd[1]");
	close_pipes(-1);
	if (!run_builtin(curr))
		execve(cmd, curr->cmds, ep);
	free(cmd);
	free_strs(ep);
	free_all(ms()->status, 0);
}
