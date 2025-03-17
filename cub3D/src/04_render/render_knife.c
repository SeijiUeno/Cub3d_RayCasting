/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_knife.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:48:30 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/17 14:23:21 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void	draw_block(t_app *app, int px, int py, uint32_t color)
{
	int			i;
	int			j;
	int			idx;
	uint32_t	*pixels;

	pixels = (uint32_t *)app->gfx.img->pixels;
	i = 0;
	while (i < KNIFE_SCALE)
	{
		j = 0;
		while (j < KNIFE_SCALE)
		{
			if (px + j >= 0 && px + j < WIDTH
				&& py + i >= 0 && py + i < HEIGHT)
			{
				idx = (py + i) * WIDTH + (px + j);
				if ((color & 0xFF) != 0)
					pixels[idx] = (color & 0xFFFFFF00) | 0xFF;
			}
			j++;
		}
		i++;
	}
}

static void	process_texture_row(t_app *app, int row, t_knife_data data)
{
	int			x;
	int			knife_width;
	uint32_t	*tex_pixels;
	uint32_t	src_color;

	knife_width = data.tex->width;
	tex_pixels = (uint32_t *)data.tex->pixels;
	x = 0;
	while (x < knife_width)
	{
		src_color = tex_pixels[row * knife_width + x];
		if ((src_color & 0xFF) != 0)
			draw_block(app, data.start.x + x * KNIFE_SCALE,
				data.start.y + row * KNIFE_SCALE, src_color);
		x++;
	}
}

void	draw_knife(t_app *app)
{
	mlx_texture_t	*tex;
	t_knife_data	data;
	int				h;
	int				r;

	tex = app->gfx.knife[app->gfx.knife_frame];
	data.tex = tex;
	data.start.x = (WIDTH - (tex->width * KNIFE_SCALE)) / 2;
	data.start.y = HEIGHT - (tex->height * KNIFE_SCALE);
	h = tex->height;
	r = 0;
	while (r < h)
	{
		process_texture_row(app, r, data);
		r++;
	}
}
