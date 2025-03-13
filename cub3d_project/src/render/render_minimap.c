/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:07 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:07:00 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "../../includes/utils.h"
#include "static_includes/raycast_utils.h"

void	draw_map_cell(t_app *app, int mx, int my, uint32_t color)
{
	int	y;
	int	x;
	int	px;
	int	py;

	y = 0;
	while (y < MINIMAP_SCALE)
	{
		x = 0;
		while (x < MINIMAP_SCALE)
		{
			px = mx * MINIMAP_SCALE + x;
			py = my * MINIMAP_SCALE + y;
			if (px >= 0 && px < (int)app->gfx.img->width
				&& py >= 0 && py < (int)app->gfx.img->height)
				mlx_put_pixel(app->gfx.img, px, py, color);
			x = x + 1;
		}
		y = y + 1;
	}
}

void	draw_map_grid(t_app *app)
{
	int			my;
	int			mx;
	uint32_t	color;

	my = 0;
	while (my < app->map.height)
	{
		mx = 0;
		while (mx < app->map.width)
		{
			if (app->map.world[my][mx] > 0)
				color = ft_pixel(0, 0, 0, 255);
			else
				color = ft_pixel(255, 255, 255, 255);
			draw_map_cell(app, mx, my, color);
			mx = mx + 1;
		}
		my = my + 1;
	}
}

static void	draw_player_icon(t_app *app, int x, int y)
{
	int	dy;
	int	dx;
	int	px;
	int	py;

	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			px = x + dx;
			py = y + dy;
			if (px >= 0 && px < (int)app->gfx.img->width && py >= 0
				&& py < (int)app->gfx.img->height)
				mlx_put_pixel(app->gfx.img, px, py, ft_pixel(255, 0, 0, 255));
			dx++;
		}
		dy++;
	}
}

void	draw_player_minimap(t_app *app)
{
	t_point	p0;
	t_point	p1;

	p0.x = (int)(app->player.posX * MINIMAP_SCALE);
	p0.y = (int)(app->player.posY * MINIMAP_SCALE);
	draw_player_icon(app, p0.x, p0.y);
	p1.x = p0.x + (int)(app->player.dirX * 8);
	p1.y = p0.y + (int)(app->player.dirY * 8);
	draw_line(app->gfx.img, p0, p1, ft_pixel(0, 0, 255, 255));
}

void	render_minimap(t_app *app)
{
	draw_map_grid(app);
	draw_player_minimap(app);
}
