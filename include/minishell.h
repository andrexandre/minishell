/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:31:55 by analexan          #+#    #+#             */
/*   Updated: 2023/12/08 16:19:30 by analexan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../srcs/libft/include/libft.h"
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

extern int			g_sig;

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
	int			*pid;
	int			fd[2];
	int			**pipe;
	int			saved_fd[2];
	char		**av;
	t_eplist	*epl;
	t_list		*words;
	t_list		*lst_lexer;
}				t_var;

// minishell
void				free_all(int exit_code);
void				free_lst(t_list *word);
void				handler(int num);
void				parsing_paths(void);

// builtin
int					run_echo(void);
int					prt_eplst(void);
int					run_export(void);
int					run_pwd(void);
int					run_unset(void);

// builtin2
t_eplist			*get_env(char *name);
int					run_cd(void);

// minishell_exec
char				**ep_from_epl(void);
void				execution(int *status);
void				cmd_execute(char *cmd, char **ep, t_list *curr);

// minishell_utils
void				*free_strs(char **strs);
void				prt_strs(char **strs, int n);
t_var				*var(void);

// lexer, parser
void				lexer(char *str);
void				parse(void);
void				search_and_replace(char *str, char src, char dest);
int					count_to_pipe(t_list *words);
int					ft_strlen_matrix(char **str);
bool				need_expande(char *str);
char				*expander(char *str);

// ep_lst
void				ep_lnew_add_back(t_eplist **lst, char *str);
void				ep_ldelone(t_eplist *lst);
void				ep_lclear(t_eplist **lst);
void				ep_export_value(char *str);
void				ep_change_value(char *name, char *data);

#endif
