/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:02:07 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/21 23:02:18 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *ft_realloc(void *ptr, size_t new_size) 
{
    void *new_ptr = malloc(new_size);
    
    if (new_ptr == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < new_size; i++) {
        ((char *)new_ptr)[i] = ((char *)ptr)[i];
    }

    free(ptr);

    return new_ptr;
}
