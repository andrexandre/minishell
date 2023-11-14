/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 19:15:44 by analexan          #+#    #+#             */
/*   Updated: 2023/11/14 22:15:40 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*builtin_str[] = {"cd", "echo", "env", "export", "pwd", "unset"};

int (*builtin_func[]) (char **) = {
	&run_cd,
	&run_echo,
	&run_env,
	&run_export,
	&run_pwd,
	&run_unset
};

int	builtin(int *status)
{
	int		i;
	t_word *word;

	i = -1;
	word = var()->lstep_parsed->content;
	while (++i < 6)
		if (!ft_strcmp(word->cmds[0], builtin_str[i]))
			return (*builtin_func[i])(word->cmds);
	if (!ft_strcmp(word->cmds[0], "exit") || !ft_strcmp(word->cmds[0], "q"))
		*status = 0;
	return (*status);
}

char	*search_cmd(char **cmdargs, char *cmd)
{
	int		i;

	i = -1;
	if (!ft_strchr(cmdargs[0], '/'))
	{
		if (var()->paths)
		{
			while (var()->paths[++i])
			{
				cmd = ft_strjoin(var()->paths[i], cmdargs[0]);
				if (!access(cmd, F_OK | X_OK))
					return (cmd);
				free(cmd);
			}
		}
	}
	else
		if (!access(cmdargs[0], F_OK | X_OK))
			return (ft_strdup(cmdargs[0]));
	perror(cmdargs[0]);
	return (NULL);
}

void	cmd_execute(char **ep)
{
	char	*cmd;
	int		pid;
	t_word *word;

	word = var()->lstep_parsed->content;

	cmd = search_cmd(word->cmds, word->cmds[0]);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		execve(cmd, word->cmds, ep);
		perror(word->cmds[0]);
		free_strs(word->cmds);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}

