/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:25 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:43:44 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static t_vertical_segment	init_vertical_segment(int x, int start_y,
	int end_y, uint32_t color)
{
	t_vertical_segment	seg;

	seg.x = x;
	seg.start_y = start_y;
	seg.end_y = end_y;
	seg.color = color;
	return (seg);
}

static void	draw_vertical_segment(mlx_image_t *img,
	const t_vertical_segment *seg)
{
	int	y;

	y = seg->start_y;
	while (y < seg->end_y)
	{
		mlx_put_pixel(img, seg->x, y, seg->color);
		y++;
	}
}

void	draw_full_column(t_app *app, t_column_draw *col_draw, t_tex_info tex)
{
	t_vertical_segment	sky;
	t_vertical_segment	floor;

	sky = init_vertical_segment(col_draw->x, 0, col_draw->draw_start,
			app->config.sky_color);
	floor = init_vertical_segment(col_draw->x, col_draw->draw_end + 1,
			HEIGHT, app->config.floor_color);
	draw_vertical_segment(app->gfx.img, &sky);
	draw_wall(app, col_draw->x, col_draw, tex);
	draw_vertical_segment(app->gfx.img, &floor);
}
