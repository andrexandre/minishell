/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:21:48 by jealves-          #+#    #+#             */
/*   Updated: 2024/01/04 23:01:25 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool is_token(char c)
{
    if (c == '|' || c == '<' || c == '>')
        return(true);
    return(false);
}

void space_token(char *str)
{
	 int i;
     int j;
     i = -1;
     while (str[++i])
     {        
        if(is_token(str[i]))
        {
            j = i;
            while (str[++j])
            {
                if(!is_token(str[j]))
                break;
            }
            ft_strrep(&str, j, j, " ");  
            ft_strrep(&str, i, i, " ");
            i = j;  
        }      
     }     
}