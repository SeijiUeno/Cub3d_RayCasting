/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:40:21 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 15:40:22 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_UTILS_H
# define FILE_UTILS_H

/* Linked list for storing map lines (during parsing) */
typedef struct s_mapLine
{
	char				*line;
	struct s_mapLine	*next;
}	t_mapLine;

/* Chunk structure for file reading */
typedef struct s_chunk
{
	char			*data;
	int				size;
	struct s_chunk	*next;
}	t_chunk;

char	*read_file_to_str(const char *filename);

#endif
