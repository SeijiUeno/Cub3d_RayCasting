/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_textured_wall.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:22 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:05:56 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <stdlib.h>
#include <math.h>

static uint32_t	convert_texture_color(uint32_t color)
{
	uint32_t	alpha;
	uint32_t	blue;
	uint32_t	green;
	uint32_t	red;

	alpha = (color >> 24) & 0xff;
	blue = (color >> 16) & 0xff;
	green = (color >> 8) & 0xff;
	red = color & 0xff;
	return ((red << 24) | (green << 16) | (blue << 8) | alpha);
}

static	uint32_t	get_tex_color(const t_tex_info tex, int tex_y)
{
	uint32_t	*pixels;
	uint32_t	texture_color;

	pixels = (uint32_t *)tex.texture->pixels;
	texture_color = pixels[tex_y * tex.texWidth + tex.texX];
	return (convert_texture_color(texture_color));
}

static uint32_t	compute_wall_pixel_color(const t_tex_info tex, int lineHeight,
	int y, int screenHeight)
{
	int		d;
	int		tex_y;

	d = y * 256 - screenHeight * 128 + lineHeight * 128;
	tex_y = ((d * tex.texHeight) / lineHeight) / 256;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= tex.texHeight)
		tex_y = tex.texHeight - 1;
	return (get_tex_color(tex, tex_y));
}

void	draw_wall(t_app *app, int x, t_column_draw *cd, t_tex_info tex)
{
	int			y;
	uint32_t	wall_color;

	y = cd->drawStart;
	while (y <= cd->drawEnd)
	{
		wall_color = compute_wall_pixel_color(tex, cd->lineHeight, y, HEIGHT);
		mlx_put_pixel(app->gfx.img, x, y, wall_color);
		y++;
	}
}
