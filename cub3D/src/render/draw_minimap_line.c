/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:27 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:43:45 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void	init_line_params_arr(t_point p0, t_point p1, int params[5])
{
	if (p1.x >= p0.x)
	{
		params[0] = p1.x - p0.x;
		params[2] = 1;
	}
	else
	{
		params[0] = p0.x - p1.x;
		params[2] = -1;
	}
	if (p1.y >= p0.y)
	{
		params[1] = p1.y - p0.y;
		params[3] = 1;
	}
	else
	{
		params[1] = p0.y - p1.y;
		params[3] = -1;
	}
	params[4] = params[0] - params[1];
}

static void	draw_line_loop(mlx_image_t *img, t_point pts[2],
	int params[5], uint32_t color)
{
	int	e2;

	while (1)
	{
		if (pts[0].x >= 0 && pts[0].x < (int)img->width
			&& pts[0].y >= 0 && pts[0].y < (int)img->height)
			mlx_put_pixel(img, pts[0].x, pts[0].y, color);
		if (pts[0].x == pts[1].x && pts[0].y == pts[1].y)
			break ;
		e2 = params[4] * 2;
		if (e2 > -params[1])
		{
			params[4] -= params[1];
			pts[0].x += params[2];
		}
		if (e2 < params[0])
		{
			params[4] += params[0];
			pts[0].y += params[3];
		}
	}
}

void	draw_line(mlx_image_t *img, t_point p0, t_point p1, uint32_t color)
{
	int		params[5];
	t_point	pts[2];

	pts[0] = p0;
	pts[1] = p1;
	init_line_params_arr(p0, p1, params);
	draw_line_loop(img, pts, params, color);
}
