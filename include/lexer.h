/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:45:30 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/06 18:19:20 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

enum				e_tokens
{
	CMD,
	AND,
	PIPE,
	WORD,
	SEMICOLON,
	REDIRECT_IN,
	REDIRECT_OUT
};
	//REDIRECT_IN_D,
	//REDIRECT_OUT_D,

typedef struct s_words
{
	enum e_tokens	token;
	char			*word;
}					t_words;

void				lexer(char *str);

#endif
