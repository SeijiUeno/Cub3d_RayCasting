/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:59 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 20:53:48 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

static void	get_line_len(const char *line, size_t *len, size_t *x_size)
{
	*len = ft_strlen(line);
	if (line[*len - 1] == '\n')
		(*len)--;
	if (*len > *x_size)
		*x_size = *len;
}

void	get_map(t_import_elements *lvl_el)
{
	size_t	len;
	int		map_started = 0;
	int		map_ended = 0;

	while ((lvl_el->line = get_next_line(lvl_el->fd)) != NULL)
	{
		// If get_next_line returns an empty string, treat it as end-of-file.
		if (ft_strlen(lvl_el->line) == 0)
		{
			free(lvl_el->line);
			break;
		}
		// Check if the line is blank (only a newline character)
		if (ft_strlen(lvl_el->line) == 1 && lvl_el->line[0] == '\n')
		{
			// If we've already started reading map rows, mark that the map has ended.
			if (map_started)
				map_ended = 1;
			free(lvl_el->line);
			continue;
		}
		// If a non-blank line appears after a blank line, that's an error.
		if (map_ended)
		{
			free(lvl_el->line);
			get_elements_error(lvl_el, "Map with blank row", 21);
		}
		// Process a valid map line.
		map_started = 1;
		get_line_len(lvl_el->line, &len, &lvl_el->lvl->x_size);
		if (len > MAX_COLS)
			get_elements_error(lvl_el, "Map MAX_COLS exceeded", 23);
		if (++(lvl_el->lvl->y_size) > MAX_ROWS)
			get_elements_error(lvl_el, "Map MAX_ROWS exceeded", 22);
		ft_memcpy(lvl_el->lvl->map[lvl_el->lvl->y_size - 1], lvl_el->line, len);
		free(lvl_el->line);
	}
	if (!lvl_el->lvl->y_size)
		get_elements_error(lvl_el, "Map missing", 20);
}
