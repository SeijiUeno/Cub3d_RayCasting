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
	rd->rayDirX = p->dirX + p->planeX * camera;
	rd->rayDirY = p->dirY + p->planeY * camera;
	rd->mapX = (int)(p->posX);
	rd->mapY = (int)(p->posY);
	if (rd->rayDirX == 0)
		rd->deltaDistX = RAY_INFINITY;
	else
		rd->deltaDistX = fabs(1 / rd->rayDirX);
	if (rd->rayDirY == 0)
		rd->deltaDistY = RAY_INFINITY;
	else
		rd->deltaDistY = fabs(1 / rd->rayDirY);
	info_x = compute_step_side(p->posX, rd->rayDirX, rd->mapX, rd->deltaDistX);
	rd->stepX = info_x.step;
	rd->side_distX = info_x.side_dist;
	info_y = compute_step_side(p->posY, rd->rayDirY, rd->mapY, rd->deltaDistY);
	rd->stepY = info_y.step;
	rd->side_distY = info_y.side_dist;
}

/* dda: Runs the DDA loop until a wall is hit.
   Parameters: (t_app *app, t_ray_data *rd)
   Returns the perpendicular wall distance.
*/

double	dda(t_app *app, t_ray_data *rd)
{
	while (1)
	{
		if (rd->side_distX < rd->side_distY)
		{
			rd->side_distX += rd->deltaDistX;
			rd->mapX += rd->stepX;
			rd->side = 0;
		}
		else
		{
			rd->side_distY += rd->deltaDistY;
			rd->mapY += rd->stepY;
			rd->side = 1;
		}
		if (rd->mapX < 0 || rd->mapX >= app->map.width
			|| rd->mapY < 0 || rd->mapY >= app->map.height)
			return (HEIGHT);
		if (app->map.world[rd->mapY][rd->mapX] > 0)
			break ;
	}
	if (rd->side == 0)
		return (rd->side_distX - rd->deltaDistX);
	else
		return (rd->side_distY - rd->deltaDistY);
}
