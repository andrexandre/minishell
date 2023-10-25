/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:16:55 by analexan          #+#    #+#             */
/*   Updated: 2023/10/25 11:34:22 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*search_cmd(char **cmdargs, int fd, int fd2)
{
	char	*cmd;
	int		i;

	i = -1;
	if (!ft_strchr(cmdargs[0], '/'))
	{
		if (vars()->paths)
		{
			while (vars()->paths[++i])
			{
				cmd = ft_strjoin(vars()->paths[i], cmdargs[0]);
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
	close_all(fd, fd2);
	free_all(0);
	exit(127);
}

void	process(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		fd;
	int		fd2;

	if (!mode)
		fd = open(av[1], O_RDONLY);
	else
		fd = vars()->pipe[mode - 1][0];
	if (mode == vars()->ac - 4)
		fd2 = open(av[vars()->ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd2 = vars()->pipe[mode][1];
	if (fd < 0 || fd2 < 0)
		error_b(3);
	cmd = search_cmd(cmdargs, fd, fd2);
	if (dup2(fd, STDIN_FILENO) < 0 || dup2(fd2, STDOUT_FILENO) < 0)
		error_b(2);
	close_all(fd, fd2);
	execve(cmd, cmdargs, ep);
	free(cmd);
	perror(cmdargs[0]);
	free_all(0);
	exit(127);
}

void	fill_args_n_pipe(int ac, char **av, char **ep)
{
	int	i;

	vars()->cmdargs = ft_calloc(ac - 2, sizeof(char *));
	vars()->cmdargs[ac - 3] = NULL;
	i = -1;
	while (++i < ac - 3)
		(vars()->cmdargs[i]) = ft_split(av[i + 2], ' ');
	vars()->pipe = ft_calloc(ac - 4, sizeof(int *));
	i = -1;
	while (++i < ac - 4)
	{
		(vars()->pipe[i]) = ft_calloc(2, sizeof(int));
		if (pipe(vars()->pipe[i]) == -1)
			error_b(1);
	}
	vars()->pids = ft_calloc(ac - 3, sizeof(int));
	i = -1;
	while (++i < ac - 3)
	{
		vars()->pids[i] = fork();
		if (vars()->pids[i] < 0)
			error_b(2);
		if (!vars()->pids[i])
			process(vars()->cmdargs[i], av, ep, i);
	}
}

void	parsing(char **ep, int i)
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
	vars()->paths = ft_split(path_from_ep, ':');
	if (!vars()->paths)
		exit(EXIT_FAILURE);
	i = -1;
	while (vars()->paths[++i])
	{
		temp = vars()->paths[i];
		(vars()->paths[i]) = ft_strjoin(temp, "/");
		free(temp);
		if (!vars()->paths[i])
			exit(EXIT_FAILURE);
	}
}

int	main(int ac, char **av, char **ep)
{
	int	i;

	vars()->ac = ac;
	vars()->av = av;
	vars()->hd = 0;
	if (ac < 5)
		error_b(0);
	if (!ft_strcmp(av[1], "here_doc"))
		here_doc(ac, av, ep);
	else
		check_fds(ac, av);
	parsing(ep, -1);
	fill_args_n_pipe(ac, av, ep);
	close_all(-1, -1);
	i = -1;
	while (++i < ac - 3)
		waitpid(vars()->pids[i], NULL, 0);
	free_all(0);
	return (0);
}

/*
cat < infile reads from the file
< infile cat reads from the file
ls > outfile writes to the file
Ex: < infile cat | wc > outfile
cat << EOF reads from stdin until reaches EOF
ls >> outfile appends to the file
Ex: wc << EOF | cat >> outfile
./pipex here_doc EOF <cmd1> <cmd2> <file>
commands to test:
unset PATH
export PATH=bixo
./pipex 123 abc comaiso tal
./pipex "" "" "" ""
./pipex infile "ping google.com -c 5" "wc -c" outfile
./pipex cat cat outfile
./pipex 0 cat cat outfile
./pipex "NULL" cat cat outfile
unset PWD || PATH
./pipex pipex.c pwd cat outfile
./pipex pipex.c "echo $PWD" cat outfile
./pipex pipex.c "echo $OLDPWD" cat outfile
< pipex.c "echo $PWD" | cat > outfile
valgrind --trace-children=yes ./pipex pipex.c pwd cat outfile
*/