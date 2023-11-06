/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/06 19:16:38 by analexan         ###   ########.fr       */
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

// cd -> bug when i do /bin it goes to /usr/bin
int	run_cd(char **cmdargs)
{
	char	*home;

	home = getenv("HOME");
	if (!cmdargs[1])
	{
		if (!home)
			prt("cd: HOME not set\n");
		else
			chdir(home);
	}
	else if (chdir(cmdargs[1]) < 0)
		perror(cmdargs[1]);
	return (1);
}
int	run_echo(char **cmdargs)
{
	if (cmdargs[1] && !ft_strncmp(cmdargs[1], "-n", 2))
		prt_strs(cmdargs + 2, ' ');
	else
	{
		prt_strs(cmdargs + 1, ' ');
		prt("\n");
	}
	return (1);
}
int	run_env(char **cmdargs)
{
	if (cmdargs[1])
		prt("env: too many arguments");
	else
		prt_strs(var()->ep, '\n');
	prt("\n");
	return (1);
}
int	run_export(char **cmdargs)
{
	(void)cmdargs;
	return (1);
}
int	run_pwd(char **cmdargs)
{
	char	*cwd;

	if (cmdargs[1])
		prt("pwd: too many arguments\n");
	else 
	{
		cwd = NULL;
		cwd = getcwd(cwd, 0);
		if (!cwd)
		{
			perror("getcwd");
			return (1);
		}
		prt("%s\n", cwd);
		free(cwd);
	}
	return (1);
}
int	run_unset(char **cmdargs)
{
	int	i;

	i = -1;
	if (cmdargs[1])
	{
		while (var()->ep[++i])
		{
			if (!ft_strcmp(var()->ep[i], cmdargs[1]))
			{
				var()->ep[i][0] = '\0';
				// use linked list and then delete the node
				break ;
			}
		}
	}
	return (1);
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
		return (1);
	while (++i < 6)
		if (!ft_strcmp(cmdargs[0], builtin_str[i]))
			return (*builtin_func[i])(cmdargs);
	if (!ft_strcmp(cmdargs[0], "exit") || !ft_strcmp(cmdargs[0], "q"))
	{
		*status = 0;
		prt("exit\n");
		return (1);
	}
	return (0);
}

void	cmd_loop(char **ep)
{
	char	**cmdargs;
	char	*buf;
	int		status;

	status = 1;
	while (status)
	{
		// prt("\033[0;34mminishell\033[0m$ ");
		prt("minishell$ ");
		buf = get_next_line(0);
		buf[ft_strlen(buf) - 1] = '\0';
		cmdargs = ft_split(buf, ' ');
		if (!builtin(cmdargs, &status))
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

int	main(int ac, char **av, char **ep)
{
	// load
	var()->ac = ac;
	var()->av = av;
	var()->ep = ep;
	parsing_paths(ep, -1);
	// loop
	cmd_loop(ep);
	// clean
	free_all();
	return (0);
}
