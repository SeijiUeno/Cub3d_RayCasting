/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:10 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:43:31 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wall_x)
{
	t_tex_info	info;

	if (rd->side == 0)
	{
		if (rd->raydir_x < 0)
			info.texture = app->gfx.tex_ea;
		else
			info.texture = app->gfx.tex_we;
	}
	else
	{
		if (rd->raydir_y < 0)
			info.texture = app->gfx.tex_so;
		else
			info.texture = app->gfx.tex_no;
	}
	info.text_wi = info.texture->width;
	info.text_heig = info.texture->height;
	info.tex_x = (int)(wall_x * info.text_wi);
	if ((rd->side == 0 && rd->raydir_x > 0)
		|| (rd->side == 1 && rd->raydir_y < 0))
		info.tex_x = info.text_wi - info.tex_x - 1;
	return (info);
}

static void	compute_wall_slice_limits(t_column_draw *col_draw,
	double perp_wall_dist)
{
	col_draw->line_h = (int)(HEIGHT / perp_wall_dist);
	if (col_draw->line_h <= 0)
		col_draw->line_h = 1;
	col_draw->draw_start = -col_draw->line_h / 2 + HEIGHT / 2;
	if (col_draw->draw_start < 0)
		col_draw->draw_start = 0;
	col_draw->draw_end = col_draw->line_h / 2 + HEIGHT / 2;
	if (col_draw->draw_end >= HEIGHT)
		col_draw->draw_end = HEIGHT - 1;
}

static void	map_wall_texture_coordinates(t_app *app, t_ray_data rd,
	t_column_draw *col_draw, double perp_wall_dist)
{
	t_tex_info	tex;

	if (rd.side == 0)
	{
		col_draw->wall_x = app->player.pos_y + perp_wall_dist * rd.raydir_y;
	}
	else
	{
		col_draw->wall_x = app->player.pos_x + perp_wall_dist * rd.raydir_x;
	}
	col_draw->wall_x = col_draw->wall_x - floor(col_draw->wall_x);
	col_draw->raydir_x = rd.raydir_x;
	col_draw->raydir_y = rd.raydir_y;
	tex = select_tex_info(app, &rd, col_draw->wall_x);
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
