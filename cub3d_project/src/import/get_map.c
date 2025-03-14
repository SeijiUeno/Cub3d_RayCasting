/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:59 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 21:22:44 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

static int	line_break(t_import_elements *lvl_el, size_t *valid_lines)
{
	if (*lvl_el->line == '\n')
	{
		if (*valid_lines)
			get_elements_error(lvl_el, "Map with blank row", 21);
		free (lvl_el->line);
		lvl_el->line = get_next_line(lvl_el->fd);
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

void	get_map(t_import_elements *lvl_el)
{
	size_t	len;

	lvl_el->line = get_next_line(lvl_el->fd);
	while (lvl_el->line)
	{
		if (line_break(lvl_el, &lvl_el->lvl->y_size)){
			printf("FIRST IF IN GET_MAP LINE_BREAK \n");
			continue ;
		}
		get_line_len(lvl_el->line, &len, &lvl_el->lvl->x_size);
		if (len > MAX_COLS)
			get_elements_error(lvl_el, "Map MAX_COLS exceeded", 23);
		if (++(lvl_el->lvl->y_size) > MAX_ROWS)
			get_elements_error(lvl_el, "Map MAX_ROWS exceeded", 22);
		ft_memcpy(lvl_el->lvl->map[lvl_el->lvl->y_size - 1], lvl_el->line, len);
		free (lvl_el->line);
		lvl_el->line = get_next_line(lvl_el->fd);
	}
	if (!lvl_el->lvl->y_size)
		get_elements_error(lvl_el, "Map missing", 20);
}
