/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/13 19:45:37 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*m_get_env(char *key)
{
	t_list	*curr;
	char 	*value;
	
	curr = var()->ep;
	value = ft_strjoin(key, "=");
	while (curr)
	{
		if (!ft_strncmp(curr->content, value, ft_strlen(value)))
		{
			free(value);
			return (curr);
		}
		curr = curr->next;
	}
	free(value);
	return (NULL);
}

int	run_cd(char **cmdargs)
{
	char	*str;
	char	*cwd;

	cwd = NULL;
	str = cmdargs[1];
	if (!cmdargs[1])
	{
		if (!m_get_env("HOME"))
		{
			prt("cd: HOME not set\n");
			return (0);
		}
		else
			str = ft_strchr(m_get_env("HOME")->content, '=') + 1;
	}
	if (!chdir(str))
	{
		str = ft_strjoin("OLDPWD=", ft_strchr(m_get_env("HOME")->content, '=') + 1);
		run_export((char *[]){"export", str, NULL});
		free(str);
		cwd = getcwd(cwd, 0);
		if (!cwd)
		{
			perror("getcwd");
			return (0);
		}
		str = ft_strjoin("PWD=", cwd);
		free(cwd);
		run_export((char *[]){"export", str, NULL});
		free(str);
	}
	else
		perror(str);
	return (0);
}

int	run_echo(char **cmdargs)
{
	int	i;

	i = 2;
	if (cmdargs[1] && !ft_strncmp(cmdargs[1], "-n", 2))
	{
		while (cmdargs[1][i] && cmdargs[1][i] == 'n')
			i++;
		if (!cmdargs[1][i])
			prt_strs(cmdargs + 2, ' ');
		else
			prt_strs(cmdargs + 1, ' ');
	}
	else
	{
		prt_strs(cmdargs + 1, ' ');
		prt("\n");
	}
	return (0);
}

int	run_env(char **cmdargs)
{
	if (cmdargs[1])
		prt("env: too many arguments\n");
	else
		print_lst(var()->ep);
	return (0);
}

int	run_export(char **cmdargs)
{
	t_list	*curr;
	char	str[500];

	while (*(++cmdargs))
	{
		curr = var()->ep;
		if (!*cmdargs)
		{
			while (curr)
			{
				// fix this so the quotes are shown in the value of the var
				prt("declare -x \"%s\"\n", curr->content);
				curr = curr->next;
			}
		}
		else if (ft_strchr(*cmdargs, '='))
		{
			ft_strlcpy(str, *cmdargs, ft_strlen(*cmdargs)
					- ft_strlen(ft_strchr(*cmdargs, '=') + 1));
			curr = m_get_env(str);
			if (curr)
			{
				free(curr->content);
				curr->content = ft_strdup(*cmdargs);
			}
			if (!curr)
				ft_lstadd_back(&var()->ep, ft_lstnew(*cmdargs));
		}
	}
	return (0);
}

int	run_pwd(char **cmdargs)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (0);
	}
	prt("%s\n", cwd);
	free(cwd);
	(void)cmdargs;
	return (0);
}

int	run_unset(char **cmdargs)
{
	t_list	*curr;

	if (!cmdargs[1])
	{
		prt("unset: not enough arguments\n");
		return (0);
	}
	while (*(++cmdargs))
	{
		curr = m_get_env(*cmdargs);
		if (curr)
		{
			if (!curr->prev)
				var()->ep = curr->next;
			else if (!curr->next)
				curr->prev->next = NULL;
			else
			{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}
			ft_lstdelone(curr, free);
		}
	}
	return (0);
}
