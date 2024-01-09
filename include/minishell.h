/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: analexan <analexan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:31:55 by analexan          #+#    #+#             */
/*   Updated: 2024/01/09 16:07:36 by analexan         ###   ########.fr       */
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
	int			debug;
	int			status;
	int			running;
	char		**paths;
	int			ac;
	int			*pid;
	int			fd[2];
	int			**pipe;
	int			saved_fd[2];
	char		*hd_buf;
	int			hd_fd;
	t_eplist	*epl;
	t_list		*words;
	t_list		*lst_lexer;
}				t_var;

// heredoc_n_handlers
void				tmp_handler(int sig);
void				handler(int sig);
int					heredoc(char *arg);

// redirects
void				redirects(t_list *curr, int *error);
int					_is_builtin(char *str);

// minishell_exec
char				**ep_from_epl(void);
void				run_child(char **ep, t_list *curr);

// minishell_loop
int					_is_builtin(char *str);
void				run_cmd(t_list *curr, int j);
int					run_builtin(t_list *curr);
void				execution(void);

// minishell_utils
void				close_pipes(int len);
void				free_pipes_words(void);
void				free_all(int exit_code, char *err_msg);
void				prt_strs(char **strs, int n);
t_var				*ms(void);

// lexer, parser
int					lexer(char *str);
void				parse(void);
void				search_and_replace(char *str, char src, char dest);
void				search_and_remove(char *str, char *target);
int					count_to_pipe(t_list *words);
int					ft_strlen_matrix(char **str);
char				*expander(char *str);
char				*space_token(char *str);
bool				validate_parse(void);

// builtin
int					run_echo(void);
int					prt_eplst(void);
int					run_unset(void);
int					run_pwd(void);
// builtin2
int					run_exit(void);
int					run_cd(void);
// export
int					ep_change_value(char *name, char *data);
int					run_export(void);
// ep_lst
t_eplist			*get_env(char *name);
void				ep_lnew(t_eplist **lst, char *str);
void				ep_ldelone(t_eplist *lst);
void				ep_lclear(t_eplist **lst);

#endif
