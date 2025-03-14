/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:11 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:30:17 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

void	ft_bzero(void *s, size_t n)
{
	char	*ptr;

	if (!n)
		return ;
	ptr = (char *)s;
	while (n-- != 0)
		*(ptr++) = '\0';
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_cpy;
	unsigned char	*src_cpy;

	if (!dest && !src)
		return (dest);
	dest_cpy = (unsigned char *)dest;
	src_cpy = (unsigned char *)src;
	while (n-- > 0)
		*(dest_cpy++) = *(src_cpy++);
	return (dest);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (!n)
		return (0);
	while ((*s1 || *s2) && (n-- > 0))
	{
		if (*(s1++) != *(s2++))
			return (*(unsigned char *)(--s1) - *(unsigned char *)(--s2));
	}
	return (0);
}
