/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:42:04 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 17:09:18 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
		i++;
	i = write(fd, s, i);
	(void)i;
}

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	ft_putstr_fd(s, fd);
	i = write(fd, "\n", 1);
	(void)i;
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned long	i;

	i = -1;
	while (++i < n)
		((unsigned char *)s)[i] = (unsigned char)c;
	return (s);
}
