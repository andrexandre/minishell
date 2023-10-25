/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 12:17:18 by analexan          #+#    #+#             */
/*   Updated: 2023/10/25 16:18:47 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
// open, close, read, write,
// access, dup, dup2, execve,
// exit, fork, pipe, unlink:
# include <unistd.h>
// malloc, free:
# include <stdlib.h>
// perror, strerror:
# include <stdio.h>
// wait, waitpid:
# include <sys/wait.h>
// flags:
# include <fcntl.h>
// va_args
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_var
{
	char	**paths;
	char	***cmdargs;
	int		**pipe;
	int		ac;
	int		hd;
	char	**av;
	pid_t	*pids;
}			t_var;

// pipex
char	*search_cmd(char **cmdargs, int fd, int fd2);
void	parsing(char **ep, int i);

// pipex_utils
void	free_all(int n);
void	close_all(int fd1, int fd2);
void	error_b(int n);
void	here_doc(int ac, char **av, char **ep);
void	check_fds(int ac, char **av);

// ft_split
char	**ft_split(char const *s, char c);
void	*free_strs(char **strs);

// get_next_line
char	*get_next_line(int fd);

// tool_lib
void	prt(char *string, ...);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);

// tool_lib2
size_t	ft_strlen(char const *s);
char	*ft_strnstr(const char *big, const char *little, size_t len);
t_var	*var(void);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strchr(const char *s, int c);

// tool_lib3
char	*ft_strdup(const char *s);
int		ft_strcmp(char *s1, char *s2);

void	free_all(int n);
void	close_all(int fd1, int fd2);
void	error_b(int n);

#endif