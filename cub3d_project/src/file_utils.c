#include "../includes/file_utils.h"
#include "../includes/structures.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef READ_BUFF_SIZE
#define READ_BUFF_SIZE 1024
#endif

static char *new_chunk_loop(ssize_t bytes_read, char *data, char *buffer) {
  int i;

  i = 0;
  while (i < bytes_read) {
    data[i] = buffer[i];
    i++;
  }
  return data;
}

void *malloc_and_free(void *alloc, size_t size) {
  alloc = malloc(sizeof(void *) * size);
  if (!alloc) {
    perror("Malloc error");
    exit(EXIT_FAILURE);
    free(alloc);
  }

  return alloc;
}

// Reads chunks from fd and returns a linked list of t_chunk.
static t_chunk *read_chunks(int fd, int *total_size) {
  t_chunk *head;
  t_chunk *tail;
  t_chunk *new_chunk;
  ssize_t bytes_read;
  char buffer[READ_BUFF_SIZE];

  head = NULL;
  tail = NULL;
  *total_size = 0;
  while ((bytes_read = read(fd, buffer, READ_BUFF_SIZE)) > 0) {
    new_chunk = malloc_and_free(new_chunk, bytes_read);
    new_chunk->data = malloc_and_free(new_chunk->data, bytes_read);
    new_chunk_loop(bytes_read, new_chunk->data, buffer);
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
static char *concat_chunks(t_chunk *head, int total_size) 
{
  char *result;
  int pos;
  int i;
  t_chunk *curr;

  pos = 0;
  result = "";
  result = malloc_and_free(result, total_size + 1);
  curr = head;
  while (curr) {
    i = 0;
    while ( i < curr->size) {
      result[pos++] = curr->data[i];
      i++;
    }
    curr = curr->next;
  }
  result[total_size] = '\0';
  return (result);
}

// Frees the chunk list.
static void free_chunks(t_chunk *head) {
  t_chunk *tmp;
  while (head) {
    tmp = head;
    head = head->next;
    free(tmp->data);
    free(tmp);
  }
}

// Main function: now only 9 lines.
char *read_file_to_str(const char *filename) {
  int fd;
  int total_size;
  t_chunk *chunks;
  char *result;

  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror("open error");
    exit(EXIT_FAILURE);
  }
  chunks = read_chunks(fd, &total_size);
  close(fd);
  result = concat_chunks(chunks, total_size);
  free_chunks(chunks);
  return (result);
}
