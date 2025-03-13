/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:10 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:48:00 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <math.h>
#include <stdlib.h>

t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wall_x)
{
	t_tex_info	info;

	if (rd->side == 0)
	{
		if (rd->raydir_x < 0)
			info.texture = app->gfx.texEast;
		else
			info.texture = app->gfx.texWest;
	}
	else
	{
		if (rd->raydir_y < 0)
			info.texture = app->gfx.texSouth;
		else
			info.texture = app->gfx.texNorth;
	}
	info.texWidth = info.texture->width;
	info.texHeight = info.texture->height;
	info.texX = (int)(wall_x * info.texWidth);
	if ((rd->side == 0 && rd->raydir_x > 0)
		|| (rd->side == 1 && rd->raydir_y < 0))
		info.texX = info.texWidth - info.texX - 1;
	return (info);
}

static void	compute_wall_slice_limits(t_column_draw *col_draw,
	double perp_wall_dist)
{
	col_draw->lineHeight = (int)(HEIGHT / perp_wall_dist);
	if (col_draw->lineHeight <= 0)
		col_draw->lineHeight = 1;

	col_draw->drawStart = -col_draw->lineHeight / 2 + HEIGHT / 2;
	if (col_draw->drawStart < 0)
		col_draw->drawStart = 0;
	col_draw->drawEnd = col_draw->lineHeight / 2 + HEIGHT / 2;
	if (col_draw->drawEnd >= HEIGHT)
		col_draw->drawEnd = HEIGHT - 1;
}

static void	map_wall_texture_coordinates(t_app *app, t_ray_data rd,
	t_column_draw *col_draw, double perp_wall_dist)
{
	t_tex_info	tex;

	if (rd.side == 0)
	{
		col_draw->wallX = app->player.pos_y + perp_wall_dist * rd.raydir_y;
	}
	else
	{
		col_draw->wallX = app->player.pos_x
 + perp_wall_dist * rd.raydir_x;
	}
	col_draw->wallX = col_draw->wallX - floor(col_draw->wallX);
	col_draw->raydir_x = rd.raydir_x;
	col_draw->raydir_y = rd.raydir_y;
	tex = select_tex_info(app, &rd, col_draw->wallX);
	draw_full_column(app, col_draw, tex);
}

void	render_column(t_app *app, int x)
{
	t_ray_data		d;
	t_column_draw	col_draw;
	double			perp_wall_dist;

	init_ray_data(app, x, &d);
	perp_wall_dist = dda(app, &d);
	if (perp_wall_dist < 0.1)
		perp_wall_dist = 0.1;
	compute_wall_slice_limits(&col_draw, perp_wall_dist);
	col_draw.x = x;
	col_draw.side = d.side;
	map_wall_texture_coordinates(app, d, &col_draw, perp_wall_dist);
}
