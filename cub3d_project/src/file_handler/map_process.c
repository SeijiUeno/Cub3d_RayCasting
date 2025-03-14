/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:32:19 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "helper/parser_helpers.h"
#include "../convert/cub3d.h"
#include <stdlib.h>
#include <stdio.h>

static void	process_map_cell(t_app *app, t_file *cube, size_t i, size_t j)
{
	char	c;

	c = cube->level.map[i][j];
	if (c >= '0' && c <= '9')
		app->map.world[i][j] = c - '0';
	else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		app->player.pos_x = j + 0.5;
		app->player.pos_y = i + 0.5;
		set_player_direction(app, c);
		app->map.world[i][j] = 0;
	}
	else if (c == ' ' || c == '\0')
		app->map.world[i][j] = 0;
	else
	{
		fprintf(stderr, "Invalid map character '%c' at (%zu, %zu)\n",
			c, i, j);
		exit(EXIT_FAILURE);
	}
}

void	fill_map(t_app *app, t_file *cube)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (j < app->map.width)
		{
			process_map_cell(app, cube, i, j);
			j++;
		}
		i++;
	}
}
