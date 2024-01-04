/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/04 22:22:25 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_token(char *str)
{
    if (ft_strcmpold(str, "|") || ft_strcmpold(str, "<") || ft_strcmpold(str, "<<") || ft_strcmpold(str, ">") || ft_strcmpold(str, ">>"))
        return(true);
}

char *space_token(char *str)
{
	 
}