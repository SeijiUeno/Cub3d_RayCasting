/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:59 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 21:22:44 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"

static int	line_break(t_file_elem *file_el, size_t *valid_lines)
{
	if (*file_el->line == '\n')
	{
		if (*valid_lines)
			error_msg_setter(file_el, "Map with blank row", 21);
		free (file_el->line);
		file_el->line = get_next_line(file_el->fd);
		return (1);
	}
	return (0);
}

static void	get_line_len(const char *line, size_t *len, size_t *x_size)
{
	*len = ft_strlen(line);
	if (line[*len - 1] == '\n')
		(*len)--;
	if (*len > *x_size)
		*x_size = *len;
}

void	set_map(t_file_elem *file_el)
{
	size_t	len;

	file_el->line = get_next_line(file_el->fd);
	while (file_el->line)
	{
		if (line_break(file_el, &file_el->src->y_size))
			continue ;
		get_line_len(file_el->line, &len, &file_el->src->x_size);
		if (len > COL)
			error_msg_setter(file_el, "Map COL exceeded", 23);
		if (++(file_el->src->y_size) > ROW)
			error_msg_setter(file_el, "Map ROW exceeded", 22);
		ft_memcpy(file_el->src->map[file_el->src->y_size - 1], file_el->line, len);
		free (file_el->line);
		file_el->line = get_next_line(file_el->fd);
	}
	if (!file_el->src->y_size)
		error_msg_setter(file_el, "Map missing", 20);
}
