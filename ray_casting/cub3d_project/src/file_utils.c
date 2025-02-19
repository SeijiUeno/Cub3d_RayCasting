#include "../includes/file_utils.h"
#include "../includes/structures.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef READ_BUFF_SIZE
# define READ_BUFF_SIZE 1024
#endif

char	*read_file_to_str(const char *filename)
{
	int			fd;
	ssize_t		bytes_read;
	char		buffer[READ_BUFF_SIZE];
	t_chunk		*head = NULL;
	t_chunk		*tail = NULL;
	int			total_size = 0;
	t_chunk		*new_chunk;
	char		*result;
	int			pos = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}
	while ((bytes_read = read(fd, buffer, READ_BUFF_SIZE)) > 0)
	{
		new_chunk = malloc(sizeof(t_chunk));
		if (!new_chunk)
		{
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		new_chunk->data = malloc(bytes_read);
		if (!new_chunk->data)
		{
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < bytes_read; i++)
			new_chunk->data[i] = buffer[i];
		new_chunk->size = bytes_read;
		new_chunk->next = NULL;
		total_size += bytes_read;
		if (!head)
			head = new_chunk;
		else
			tail->next = new_chunk;
		tail = new_chunk;
	}
	if (bytes_read < 0)
	{
		perror("read error");
		exit(EXIT_FAILURE);
	}
	close(fd);
	result = malloc(total_size + 1);
	if (!result)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for (t_chunk *curr = head; curr; )
	{
		for (int i = 0; i < curr->size; i++)
			result[pos++] = curr->data[i];
		t_chunk *tmp = curr;
		curr = curr->next;
		free(tmp->data);
		free(tmp);
	}
	result[total_size] = '\0';
	return (result);
}
