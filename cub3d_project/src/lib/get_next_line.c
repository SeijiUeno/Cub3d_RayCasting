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

static char	*ft_merge_till_bslash_n(char *line, t_buffer *buf)
{
	size_t	i;
	size_t	n_right;
	char	*merged;

	i = buf->pos;
	while (buf->buffer[i] && buf->buffer[i] != '\n')
		i++;
	if (buf->buffer[i] == '\n')
	{
		i++;
		buf->bslash = 1;
	}
	n_right = i - buf->pos;
	merged = merge_n_right(line, &buf->buffer[buf->pos], n_right);
	buf->pos = i;
	return (merged);
}

static char	*line_iterative(t_buffer *buf, int fd, char *line)
{
	ssize_t	read_size;

	while (1)
	{
		if (!buf->pos)
		{
			read_size = read(fd, buf->buffer, BUFFER_SIZE);
			if (read_size == -1 || (!read_size && !*line))
			{
				free(line);
				return (NULL);
			}
			if (!read_size)
				return (line);
		}
		line = ft_merge_till_bslash_n(line, buf);
		if (!line || buf->bslash)
			return (line);
		buf->pos = 0;
		ft_bzero(buf->buffer, BUFFER_SIZE);
	}
}

char	*get_next_line(int fd)
{
	static t_buffer	*arr[1024];
	char			*line;

	if (fd < 0 || BUFFER_SIZE < 1 || fd >= 1024)
		return (NULL);
	if (!arr[fd])
		arr[fd] = ft_calloc(1, sizeof(t_buffer));
	if (!arr[fd])
		return (NULL);
	arr[fd]->bslash = 0;
	line = ft_calloc(1, sizeof(char));
	if (line)
		line = line_iterative(arr[fd], fd, line);
	if (!line)
	{
		free (arr[fd]);
		arr[fd] = NULL;
	}
	return (line);
}
