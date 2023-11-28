/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/28 15:08:42 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef struct s_eplist
{
	char			*str;
	char			*name;
	char			*data;
	struct s_eplist	*prev;
	struct s_eplist	*next;
}					t_eplist;

typedef struct s_var
{
	int			status;
	char		**paths;
	int			ac;
	char		**av;
	char		**cmdargs;
	t_eplist	*epl;
	t_list		*words;
	t_list		*lst_lexer;
}				t_var;

// minishell
void				free_all(void);
void				free_lst(t_list *word);

// builtin
int					run_echo(void);
int					run_env(void);
int					run_export(void);
int					run_pwd(void);
int					run_unset(void);

// builtin2
int					run_cd(void);
void				ep_change_value(char *key, char *value);

// minishell_exec
int					builtin(int *status);
void				cmd_execute(char **ep);

// minishell_utils
void				print_lst(t_list *lst, int n);
void				*free_strs(char **strs);
void				prt_strs(char **strs, char sep);
t_var				*var(void);

// lexer, parser
void				lexer(char *str);
void				parse(void);

// ep_lst
t_eplist			*ep_lnew(char *str);
void				ep_ladd_back(t_eplist **lst, t_eplist *new);
void				ep_ldelone(t_eplist *lst);
void				ep_lclear(t_eplist **lst);
void				print_eplst(t_eplist *lst);

// ep_lst2
t_eplist			*get_env(char *key);
t_eplist			*ep_export_value(char *str);

#endif
