# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

void	close_all(int **pipe, int num_of_cmds)
{
	int	i;

	i = -1;
	while (++i < num_of_cmds - 1)
	{
		close(pipe[i][0]);
		close(pipe[i][1]);
	}
}

int main(int ac, char **av, char **ep)
{
	(void)ac;
	(void)av;
	char ***cmds = calloc(sizeof(char *), 3);
	cmds[2] = NULL;
	cmds[0] = calloc(sizeof(char *), 3);
	cmds[0][0] = "cat";
	cmds[0][1] = "../readline.supp";
	cmds[0][2] = NULL;
	cmds[1] = calloc(sizeof(char *), 3);
	cmds[1][0] = "grep";
	cmds[1][1] = "leak";
	cmds[1][2] = NULL;
	int	num_of_cmds = 2;
	int	**pipe_fd = calloc(num_of_cmds - 1, sizeof(int *));
	for (int i = 0; i < num_of_cmds - 1; i++)
	{
		pipe_fd[i] = calloc(2, sizeof(int));
		if (pipe(pipe_fd[i]) == -1)
			perror("pipe");
	}
	int	pid[num_of_cmds];
	for (int i = 0; i < num_of_cmds; i++)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			perror("fork");
		if (!pid[i])
		{
			if (i && dup2(pipe_fd[i - 1][0], STDIN_FILENO) < 0)
				perror("dup2");
			if (i != num_of_cmds - 1 && dup2(pipe_fd[i][1], STDOUT_FILENO) < 0)
				perror("dup2");
			close_all(pipe_fd, num_of_cmds);
			if (!i)
				execve("/bin/cat", cmds[i], ep);
			if (i)
				execve("/bin/grep", cmds[i], ep);
		}
		free(cmds[i]);
	}
	free(cmds);
	close_all(pipe_fd, num_of_cmds);
	int	i = -1;
	while (++i < num_of_cmds - 1)
		free(pipe_fd[i]);
	free(pipe_fd);
	return (0);
}
