/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 11:18:18 by analexan          #+#    #+#             */
/*   Updated: 2023/11/09 15:10:28 by jealves-         ###   ########.fr       */
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

// char	**cmdargs;
typedef struct s_var
{
	int		signal;
	char	**paths;
	int		ac;
	char	**av;
	char	**ep;
	t_list	*lstep;
	t_list	*words;
	t_list	*lstep_parsed;
}			t_var;

// minishell
int			run_cd(char **cmdargs);
int			run_echo(char **cmdargs);
int			run_env(char **cmdargs);
int			run_export(char **cmdargs);
int			run_pwd(char **cmdargs);
int			run_unset(char **cmdargs);
char		*m_get_env(char *key);

void		lexer(char *str);
void	parse();
t_var		*var(void);

#endif
