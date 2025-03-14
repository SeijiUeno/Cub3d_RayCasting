/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:51:17 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:53:27 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

char	*prepend_prefix(const char *prefix, const char *s)
{
	size_t	len_prefix;
	size_t	len_s;
	char	*result;
	size_t	i;
	size_t	j;

	len_prefix = ft_strlen(prefix);
	len_s = ft_strlen(s);
	result = malloc(len_prefix + len_s + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (prefix[i])
	{
		result[i] = prefix[i];
		i++;
	}
	j = 0;
	while (s[j])
	{
		result[i + j] = s[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}