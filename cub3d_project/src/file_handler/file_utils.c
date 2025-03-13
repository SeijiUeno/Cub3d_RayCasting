/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:42:40 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 15:48:43 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/file_utils.h"
#include "../../includes/structures.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef READ_BUFF_SIZE
# define READ_BUFF_SIZE 1024
#endif

// Reads chunks from fd and returns a linked list of t_chunk.
// (≤ 25 lines)
static t_chunk	*read_chunks(int fd, int *total_size)
{
	t_chunk	*head = NULL;
	t_chunk	*tail = NULL;
	t_chunk	*new_chunk;
	ssize_t	bytes_read;
	char	buffer[READ_BUFF_SIZE];

	*total_size = 0;
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
		*total_size += bytes_read;
		if (!head)
			head = new_chunk;
		else
			tail->next = new_chunk;
		tail = new_chunk;
	}
	return (head);
}

// Concatenates the chunks into one allocated string.
// (≤ 25 lines)
static char	*concat_chunks(t_chunk *head, int total_size)
{
	char	*result;
	int		pos = 0;
	t_chunk	*curr;

	result = malloc(total_size + 1);
	if (!result)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	for (curr = head; curr; curr = curr->next)
	{
		for (int i = 0; i < curr->size; i++)
			result[pos++] = curr->data[i];
	}
	result[total_size] = '\0';
	return (result);
}

// Frees the chunk list.
static void	free_chunks(t_chunk *head)
{
	t_chunk *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->data);
		free(tmp);
	}
}

// Main function: now only 9 lines.
char	*read_file_to_str(const char *filename)
{
	int		fd;
	int		total_size;
	t_chunk	*chunks;
	char	*result;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}
	chunks = read_chunks(fd, &total_size);
	close(fd);
	result = concat_chunks(chunks, total_size);
	free_chunks(chunks);
	return (result);
}
