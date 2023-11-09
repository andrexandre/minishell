/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:45:30 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/08 19:07:51 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>

enum			e_type
{
	WORD,
	TOKEN,
	BUILD_IN
};
//AND,
//PIPE,
//SEMICOLON,
//REDIRECT_IN,
//REDIRECT_OUT
//REDIRECT_IN_D,
//REDIRECT_OUT_D,

typedef struct s_word
{
	enum e_type	type;
	char		*str;
	bool 	is_builin;
}				t_word;


#endif
