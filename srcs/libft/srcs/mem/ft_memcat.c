/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealves- <jealves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 22:36:07 by jealves-          #+#    #+#             */
/*   Updated: 2023/11/28 20:47:51 by jealves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcat(void *v1, void *v2)
{
	size_t len;
	size_t i;
	size_t len_s1;
	char *s1;
	char *s2;
	char *dest;

	i = 0;
	s1 = (char *)v1;
	s2 = (char *)v2;
	len_s1 = ft_strlen(s1);
	len = len_s1 + ft_strlen(s2);
	dest = ft_calloc(len, sizeof(s1));
	ft_memcpy(dest, s1 , len_s1);
	while (i <= ft_strlen(s2))
	{
		dest[len_s1] = s2[i];
		len_s1++;
		i++;
	}
	free(s1);
	return (dest);
}

/*int main()
{
    char src[] = "Jessica";
    char dest[30] = "meu nome é ";
    size_t i = ft_strlcat(dest,src,20);

    printf("%ld\n", i);
    printf("%s", dest);
}*/
