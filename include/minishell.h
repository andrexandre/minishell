/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/16 13:36:14 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lexer.h"
# include "libft.h"
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

typedef struct s_word
{
	enum e_type	type;
	char		*str;
	bool		is_builtin;
	char		**cmds;
}				t_word;

typedef struct s_var
{
	int			signal;
	char		**paths;
	int			ac;
	char		**av;
	t_list		*ep;
	t_list		*words;
	t_list		*lstep_parsed;
}				t_var;

// minishell
void	free_all(void);

// builtin
int				run_cd(char **cmdargs);
int				run_echo(char **cmdargs);
int				run_env(char **cmdargs);
int				run_export(char **cmdargs);
int				run_pwd(char **cmdargs);
int				run_unset(char **cmdargs);
t_list			*m_get_env(char *key);

// minishell_exec
int				builtin(int *status);
void			cmd_execute(char **ep);

// minishell_utils
void			print_lst(t_list *lst);
void			*free_strs(char **strs);
void			prt_strs(char **strs, char sep);
t_var			*var(void);

// lexer, parser
void			lexer(char *str);
void			parse(void);

#endif
