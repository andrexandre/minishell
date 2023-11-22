/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 18:36:21 by analexan          #+#    #+#             */
/*   Updated: 2023/10/27 20:35:14 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	check_fds(int ac, char **av)
{
	int		fd;
	int		fd2;

	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		perror(av[1]);
	else
		close(fd);
	fd2 = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd2 == -1)
		perror(av[ac - 1]);
	else
		close(fd2);
	if (fd == -1 || fd2 == -1)
		exit(EXIT_FAILURE);
}

// receive input from the stdin and save in in the pipe
void	get_stdin(void)
{
	char	*buf;
	char	*str;

	ft_printf("pipe heredoc>");
	buf = get_next_line(STDIN_FILENO);
	str = ft_strjoin(var()->av[2], "\n");
	while (ft_strcmp(buf, str))
	{
		write(var()->pipe[0][1], buf, ft_strlen(buf));
		free(buf);
		ft_printf("pipe heredoc>");
		buf = get_next_line(STDIN_FILENO);
	}
	free(str);
	free(buf);
}

void	process_hd(char **cmdargs, char **av, char **ep, int mode)
{
	char	*cmd;
	int		fd;
	int		fd2;

	fd = var()->pipe[mode][0];
	if (mode)
		fd2 = open(av[var()->ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd2 = var()->pipe[1][1];
	if (mode && fd2 < 0)
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

void	fill_args_n_pipe_hd(char **av, char **ep)
{
	int	i;

	var()->cmdargs = ft_calloc(3, sizeof(char *));
	var()->cmdargs[2] = NULL;
	(var()->cmdargs[0]) = ft_split(av[3], ' ');
	(var()->cmdargs[1]) = ft_split(av[4], ' ');
	var()->pipe = ft_calloc(2, sizeof(int *));
	(var()->pipe[0]) = ft_calloc(2, sizeof(int));
	if (pipe(var()->pipe[0]) == -1)
		error_b(1);
	(var()->pipe[1]) = ft_calloc(2, sizeof(int));
	if (pipe(var()->pipe[1]) == -1)
		error_b(1);
	get_stdin();
	var()->pids = ft_calloc(2, sizeof(int));
	i = -1;
	while (++i < 2)
	{
		var()->pids[i] = fork();
		if (var()->pids[i] < 0)
			error_b(2);
		if (!var()->pids[i])
			process_hd(var()->cmdargs[i], av, ep, i);
	}
}

void	here_doc(int ac, char **av, char **ep)
{
	int		fd;
	int		i;

	var()->hd = 1;
	if (ac != 6)
		error_b(4);
	fd = open(av[var()->ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		perror(av[var()->ac - 1]);
	else
		close(fd);
	if (fd == -1)
		exit(EXIT_FAILURE);
	parsing(ep, -1);
	fill_args_n_pipe_hd(av, ep);
	close_all(-1, -1);
	i = -1;
	while (++i < 2)
		waitpid(var()->pids[i], NULL, 0);
	free_all(0);
	exit(EXIT_SUCCESS);
}
