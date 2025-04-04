/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_allocation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:24:33 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 16:31:56 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

void	allocate_map(t_app *app, t_file *cube)
{
	size_t	i;

	app->map.width = cube->level.x_size;
	app->map.height = cube->level.y_size;
	app->map.world = malloc(sizeof(int *) * app->map.height);
	if (!app->map.world)
	{
		ft_error("malloc error");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < app->map.height)
	{
		app->map.world[i] = malloc(sizeof(int) * app->map.width);
		if (!app->map.world[i])
		{
			ft_error("malloc error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
