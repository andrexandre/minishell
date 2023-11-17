/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:10:35 by analexan          #+#    #+#             */
/*   Updated: 2023/11/17 18:46:00 by analexan         ###   ########.fr       */
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

int	run_cd(void)
{
	char	*str;
	// char	*cwd;

	// cwd = NULL;
	if (!var()->words->next)
	{
		if (!m_get_env("HOME"))
		{
			prt("cd: HOME not set\n");
			return (0);
		}
		else
			str = ft_strchr(m_get_env("HOME")->content, '=') + 1;
	}
	else
		str = var()->words->next->content;
	if (!chdir(str))
	{
		// str = ft_strjoin("OLDPWD=", ft_strchr(m_get_env("HOME")->content, '=') + 1);
		// run_export((char *[]){"export", str, NULL});
		// free(str);
		// cwd = getcwd(cwd, 0);
		// if (!cwd)
		// {
		// 	perror("getcwd");
		// 	return (0);
		// }
		// str = ft_strjoin("PWD=", cwd);
		// free(cwd);
		// run_export((char *[]){"export", str, NULL});
		// free(str);
	}
	else
		perror(str);
	return (0);
}

int	run_echo(void)
{
	int		i;
	char	*str;

	i = 2;
	if (var()->words->next && !ft_strncmp(var()->words->next->content, "-n", 2))
	{
		str = var()->words->next->content;
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			print_lst(var()->words->next->next, 1);
		else
			print_lst(var()->words->next, 0);
	}
	else
		print_lst(var()->words->next, 0);
	return (0);
}

int	run_env(void)
{
	if (var()->words->next)
		prt("env: too many arguments\n");
	else
		print_lst(var()->ep, 0);
	return (0);
}

int	run_export(void)
{
	t_list	*curr;
	t_list	*new;
	char	str[500];

	curr = var()->ep;
	if (!var()->words->next)
	{
		while (curr)
		{
			ft_strlcpy(str, curr->content, ft_strlen(curr->content)
					- ft_strlen(ft_strchr(curr->content, '=')) + 1);
			prt("declare -x %s=\"%s\"\n", str, ft_strchr(curr->content, '=') + 1);
			curr = curr->next;
		}
		return (0);
	}
	new = var()->words;
 	while (new)
	{
		if (ft_strchr(new->content, '='))
		{
			ft_strlcpy(str, new->content, ft_strlen(new->content)
					- ft_strlen(ft_strchr(new->content, '=') + 1));
			curr = m_get_env(str);
			if (curr)
			{
				free(curr->content);
				curr->content = ft_strdup(new->content);
			}
			if (!curr)
			{
				new = ft_lstnew(ft_strdup(new->content));
				ft_lstadd_back(&var()->ep, new);
				// new->name = ft_substr(new->content, 0, ft_strlen(new->content)
				// 		- ft_strlen(ft_strchr(new->content, '=')));
				// new->data = ft_strchr(new->content, '=') + 1;
			}
		}
		new = new->next;
	}
	return (0);
}

int	run_pwd(void)
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
	return (0);
}

int	run_unset(void)
{
	t_list	*curr;
	t_list	*new;

	new = var()->words->next;
	while (new)
	{
		curr = m_get_env(new->content);
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
		new = new->next;
	}
	return (0);
}
