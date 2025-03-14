/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:15 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 19:27:38 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static t_step_info	compute_step_side(double pos, double rayDir,
	int mapCoord, double deltaDist)
{
	t_step_info	info;

	if (rayDir < 0)
	{
		info.step = -1;
		info.side_dist = (pos - mapCoord) * deltaDist;
	}
	else
	{
		info.step = 1;
		info.side_dist = ((mapCoord + 1.0) - pos) * deltaDist;
	}
	return (info);
}

void	init_ray_data(t_app *app, int x, t_ray_data *rd)
{
	t_player	*p;
	t_step_info	info_x;
	t_step_info	info_y;
	double		camera;

	p = &app->player;
	camera = 2.0 * x / (double)WIDTH - 1.0;
	rd->raydir_x = p->dir_x + p->plan_x * camera;
	rd->raydir_y = p->dir_y + p->plan_y * camera;
	rd->mp_x = (int)(p->pos_x
);
	rd->mp_y = (int)(p->pos_y);
	if (rd->raydir_x == 0)
		rd->deltad_x = RAY_INFINITY;
	else
		rd->deltad_x = fabs(1 / rd->raydir_x);
	if (rd->raydir_y == 0)
		rd->deltad_y = RAY_INFINITY;
	else
		rd->deltad_y = fabs(1 / rd->raydir_y);
	info_x = compute_step_side(p->pos_x
, rd->raydir_x, rd->mp_x, rd->deltad_x);
	rd->step_x = info_x.step;
	rd->side_dist_x = info_x.side_dist;
	info_y = compute_step_side(p->pos_y, rd->raydir_y, rd->mp_y, rd->deltad_y);
	rd->step_y = info_y.step;
	rd->side_dist_y = info_y.side_dist;
}

double	dda(t_app *app, t_ray_data *rd)
{
	while (1)
	{
		if (rd->side_dist_x < rd->side_dist_y)
		{
			rd->side_dist_x += rd->deltad_x;
			rd->mp_x += rd->step_x;
			rd->side = 0;
		}
		else
		{
			rd->side_dist_y += rd->deltad_y;
			rd->mp_y += rd->step_y;
			rd->side = 1;
		}
		if (rd->mp_x < 0 || rd->mp_x >= (int)app->map.width
			|| rd->mp_y < 0 || rd->mp_y >= (int)app->map.height)
			return (HEIGHT);
		if (app->map.world[rd->mp_y][rd->mp_x] > 0)
			break ;
	}
	if (rd->side == 0)
		return (rd->side_dist_x - rd->deltad_x);
	else
		return (rd->side_dist_y - rd->deltad_y);
}
