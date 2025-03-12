#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wallX)
{
	t_tex_info	info;

	if (rd->side == 0)
	{
		if (rd->rayDirX < 0)
			info.texture = app->gfx.texEast;
		else
			info.texture = app->gfx.texWest;
	}
	else
	{
		if (rd->rayDirY < 0)
			info.texture = app->gfx.texSouth;
		else
			info.texture = app->gfx.texNorth;
	}
	info.texWidth = info.texture->width;
	info.texHeight = info.texture->height;
	info.texX = (int)(wallX * info.texWidth);
	if ((rd->side == 0 && rd->rayDirX > 0)
		|| (rd->side == 1 && rd->rayDirY < 0))
		info.texX = info.texWidth - info.texX - 1;
	return (info);
}

static t_step_info	compute_step_side(double pos, double rayDir,
	int mapCoord, double deltaDist)
{
	t_step_info	info;

	if (rayDir < 0)
	{
		info.step = -1;
		info.sideDist = (pos - mapCoord) * deltaDist;
	}
	else
	{
		info.step = 1;
		info.sideDist = ((mapCoord + 1.0) - pos) * deltaDist;
	}
	return (info);
}

void	init_ray_data(t_app *app, int x, t_ray_data *rd)
{
	t_player	*p;
	t_step_info	infoX;
	t_step_info	infoY;
	double		cameraX;

	p = &app->player;
	cameraX = 2.0 * x / (double)WIDTH - 1.0;
	rd->rayDirX = p->dirX + p->planeX * cameraX;
	rd->rayDirY = p->dirY + p->planeY * cameraX;
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
	infoX = compute_step_side(p->posX, rd->rayDirX, rd->mapX, rd->deltaDistX);
	rd->stepX = infoX.step;
	rd->sideDistX = infoX.sideDist;
	infoY = compute_step_side(p->posY, rd->rayDirY, rd->mapY, rd->deltaDistY);
	rd->stepY = infoY.step;
	rd->sideDistY = infoY.sideDist;
}

/* dda: Runs the DDA loop until a wall is hit.
   Parameters: (t_app *app, t_ray_data *rd)
   Returns the perpendicular wall distance.
*/

double	dda(t_app *app, t_ray_data *rd)
{
	while (1)
	{
		if (rd->sideDistX < rd->sideDistY)
		{
			rd->sideDistX += rd->deltaDistX;
			rd->mapX += rd->stepX;
			rd->side = 0;
		}
		else
		{
			rd->sideDistY += rd->deltaDistY;
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
		return (rd->sideDistX - rd->deltaDistX);
	else
		return (rd->sideDistY - rd->deltaDistY);
}
