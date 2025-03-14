/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:26 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:54:33 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	total = nmemb * size;
	if (nmemb && total / nmemb != size)
		return (NULL);
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}

static char	*merge_n_right(char *left, char *right, size_t n_right)
{
	char	*merged;
	size_t	size;

	size = ft_strlen(left) + n_right + 1;
	merged = malloc(size * sizeof(char));
	if (merged)
	{
		merged[--size] = '\0';
		while (n_right-- && size--)
			merged[size] = right[n_right];
		while (size--)
			merged[size] = left[size];
	}
	free(left);
	return (merged);
}

static char	*merge_slash(char *line, t_bffer *bf)
{
	size_t	i;
	size_t	n_right;
	char	*merged;

	i = bf->pos;
	while (bf->bffer[i] && bf->bffer[i] != '\n')
		i++;
	if (bf->bffer[i] == '\n')
	{
		i++;
		bf->bslash = 1;
	}
	n_right = i - bf->pos;
	merged = merge_n_right(line, &bf->bffer[bf->pos], n_right);
	bf->pos = i;
	return (merged);
}

static char	*iter_line(t_bffer *bf, int fd, char *line)
{
	ssize_t	read_size;

	while (1)
	{
		if (!bf->pos)
		{
			read_size = read(fd, bf->bffer, BUFFER);
			if (read_size == -1 || (!read_size && !*line))
			{
				free(line);
				return (NULL);
			}
			if (!read_size)
				return (line);
		}
		line = merge_slash(line, bf);
		if (!line || bf->bslash)
			return (line);
		bf->pos = 0;
		ft_bzero(bf->bffer, BUFFER);
	}
}

char	*get_next_line(int fd)
{
	static t_bffer	*arr[1024];
	char			*line;

	if (fd < 0 || BUFFER < 1 || fd >= 1024)
		return (NULL);
	if (!arr[fd])
		arr[fd] = ft_calloc(1, sizeof(t_bffer));
	if (!arr[fd])
		return (NULL);
	arr[fd]->bslash = 0;
	line = ft_calloc(1, sizeof(char));
	if (line)
		line = iter_line(arr[fd], fd, line);
	if (!line)
	{
		free (arr[fd]);
		arr[fd] = NULL;
	}
	return (line);
}
