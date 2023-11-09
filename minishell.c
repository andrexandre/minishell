/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/09 15:08:01 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prt_strs(char **strs, char sep)
{
	int	i;

	i = -1;
	if (!strs)
		return ;
	while (strs[++i])
	{
		prt("%s", strs[i]);
		if (strs[i + 1])
			prt("%c", sep);
	}
}

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

void	free_all(void)
{
	t_list	*current;

	while (var()->lstep)
	{
		current = var()->lstep->next;
		ft_lstdelone(var()->lstep, free);
		var()->lstep = current;
	}
	free_strs(var()->paths);
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

void	cmd_execute(char **cmdargs, char **ep)
{
	char	*cmd;
	int		pid;

	cmd = search_cmd(cmdargs, cmdargs[0]);
	if (!cmd)
		return ;
	pid = fork();
	if (pid < 0)
		perror("fork");
	if (!pid)
	{
		execve(cmd, cmdargs, ep);
		perror(cmdargs[0]);
		free_strs(cmdargs);
		free_all();
		exit(127);
	}
	wait(0);
	free(cmd);
}

void	print_lst(t_list *lst)
{
	while (lst)
	{
		prt("%s\n", lst->content);
		lst = lst->next;
	}
}

char	*m_get_env(char *key)
{
	t_list	*curr;
	char 	*value;
	
	curr = var()->lstep;
	value = ft_strjoin(key, "=");
	while (curr)
	{
		if (!ft_strncmp(curr->content, value, ft_strlen(value)))
		{
			free(value);
			return (ft_strchr(curr->content, '=') + 1);
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
			str = m_get_env("HOME");
	}
	if (!chdir(str))
	{
		str = ft_strjoin("OLDPWD=", m_get_env("PWD"));
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
		print_lst(var()->lstep);
	return (0);
}
int	run_export(char **cmdargs)
{
	t_list	*curr;

	curr = var()->lstep;
	if (!cmdargs[1])
	{
		while (curr)
		{
			// fix this so the quotes are shown in the value of the var
			prt("declare -x \"%s\"\n", curr->content);
			curr = curr->next;
		}
	}
	else if (ft_strchr(cmdargs[1], '='))
	{
		while (curr)
		{
			if (!ft_strncmp(curr->content, cmdargs[1], ft_strlen(cmdargs[1])
					- ft_strlen(ft_strchr(cmdargs[1], '=') + 1)))
			{
				free(curr->content);
				curr->content = ft_strdup(cmdargs[1]);
				break ;
			}
			curr = curr->next;
		}
		if (!curr)
			ft_lstadd_back(&var()->lstep, ft_lstnew(cmdargs[1]));
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
	t_list	*prev;
	t_list	*curr;

	if (cmdargs[1])
	{
		prev = NULL;
		curr = var()->lstep;
		while (curr)
		{
			if (!ft_strncmp(curr->content, cmdargs[1], ft_strlen(cmdargs[1])))
			{
				if (!prev)
					var()->lstep = curr->next;
				else
					prev->next = curr->next;
				ft_lstdelone(curr, free);
				return (0);
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return (0);
}

char *builtin_str[] = {
	"cd",
	"echo",
	"env",
	"export",
	"pwd",
	"unset"
};

int (*builtin_func[]) (char **) = {
	&run_cd,
	&run_echo,
	&run_env,
	&run_export,
	&run_pwd,
	&run_unset
};

int	builtin(char **cmdargs, int *status)
{
	int		i;

	i = -1;
	if (!cmdargs[0])
		return (0);
	while (++i < 6)
		if (!ft_strcmp(cmdargs[0], builtin_str[i]))
			return (*builtin_func[i])(cmdargs);
	if (!ft_strcmp(cmdargs[0], "exit") || !ft_strcmp(cmdargs[0], "q"))
	{
		*status = 0;
		prt("exit\n");
		return (0);
	}
	return (1);
}
void init_minishell(void)
{
	var()->words = NULL;
	var()->parsing_words = NULL;
}

void	cmd_loop(char **ep)
{
	char	**cmdargs;
	char	*buf;
	int		status;
	status = 1;
	while (status)
	{
		buf = readline("\033[0;34mminishell\033[0m$ ");
		cmdargs = ft_split(buf, ' ');
		add_history(buf);
		init_minishell();
		lexer(buf);
		parse();
		if (builtin(cmdargs, &status))
			cmd_execute(cmdargs, ep);
		free(buf);
		free_strs(cmdargs);
	}
}

void	parsing_paths(char **ep, int i)
{
	char	*path_from_ep;
	char	*temp;

	path_from_ep = NULL;
	while (ep[++i])
	{
		path_from_ep = ft_strnstr(ep[i], "PATH=", 5);
		if (path_from_ep)
			break ;
	}
	if (!path_from_ep)
		return ;
	path_from_ep += 5;
	var()->paths = ft_split(path_from_ep, ':');
	if (!var()->paths)
		exit(EXIT_FAILURE);
	i = -1;
	while (var()->paths[++i])
	{
		temp = var()->paths[i];
		(var()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!var()->paths[i])
			exit(EXIT_FAILURE);
	}
}

void	create_lstep(char **ep)
{
	int		i;
	char	*cwd;
	char	*str;

	i = -1;
	cwd = NULL;
	while (ep[++i])
		ft_lstadd_back(&var()->lstep, ft_lstnew(ep[i]));
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	str = ft_strjoin("OLDPWD=", cwd);
	run_export((char *[]){"export", str, NULL});
	free(str);
	str = ft_strjoin("PWD=", cwd);
	run_export((char *[]){"export", str, NULL});
	free(str);
	free(cwd);
}

int	main(int ac, char **av, char **ep)
{
	var()->ac = ac;
	var()->av = av;
	var()->ep = ep;
	create_lstep(ep);
	parsing_paths(ep, -1);
	cmd_loop(ep);
	free_all();
	return (0);
}
