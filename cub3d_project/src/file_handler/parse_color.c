/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:44:48 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../import/cub3d.h"

void	parse_colors(t_app *app, t_file *cube)
{
	app->config.floorColor = ft_pixel(cube->level.f[0],
			cube->level.f[1], cube->level.f[2], 255);
	app->config.ceilingColor = ft_pixel(cube->level.c[0],
			cube->level.c[1], cube->level.c[2], 255);
}
