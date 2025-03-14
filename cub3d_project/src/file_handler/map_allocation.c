/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:24:33 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "helper/parser_helpers.h"
#include "../convert/cub3d.h"
#include <stdlib.h>
#include <stdio.h>

void	allocate_map(t_app *app, t_file *cube)
{
	size_t	i;

	app->map.width = cube->level.x_size;
	app->map.height = cube->level.y_size;
	app->map.world = malloc(sizeof(int *) * app->map.height);
	if (!app->map.world)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < app->map.height)
	{
		app->map.world[i] = malloc(sizeof(int) * app->map.width);
		if (!app->map.world[i])
		{
			perror("malloc error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
