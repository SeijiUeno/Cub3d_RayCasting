/* src/raycasting_utils.c */
#include "../../includes/render.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void init_ray_data(t_app *app, int x, t_ray_data *rd)
{
	t_player *p = &app->player;
	double cameraX = 2.0 * x / (double)WIDTH - 1.0;
	rd->rayDirX = p->dirX + p->planeX * cameraX;
	rd->rayDirY = p->dirY + p->planeY * cameraX;
	rd->mapX = (int)(p->posX);
	rd->mapY = (int)(p->posY);
	if (rd->rayDirX == 0)
		rd->deltaDistX = 1e30;
	else
		rd->deltaDistX = fabs(1 / rd->rayDirX);
	if (rd->rayDirY == 0)
		rd->deltaDistY = 1e30;
	else
		rd->deltaDistY = fabs(1 / rd->rayDirY);
	/* Compute step and initial side distances */
	if (rd->rayDirX < 0)
	{
		rd->stepX = -1;
		rd->sideDistX = (p->posX - rd->mapX) * rd->deltaDistX;
	}
	else
	{
		rd->stepX = 1;
		rd->sideDistX = ((rd->mapX + 1.0) - p->posX) * rd->deltaDistX;
	}
	if (rd->rayDirY < 0)
	{
		rd->stepY = -1;
		rd->sideDistY = (p->posY - rd->mapY) * rd->deltaDistY;
	}
	else
	{
		rd->stepY = 1;
		rd->sideDistY = ((rd->mapY + 1.0) - p->posY) * rd->deltaDistY;
	}
}

/* dda: Runs the DDA loop until a wall is hit.
   Parameters: (t_app *app, t_ray_data *rd)
   Returns the perpendicular wall distance.
*/
double dda(t_app *app, t_ray_data *rd)
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
		if (rd->mapX < 0 || rd->mapX >= app->map.width ||
		    rd->mapY < 0 || rd->mapY >= app->map.height)
			return HEIGHT;
		if (app->map.world[rd->mapY][rd->mapX] > 0)
			break;
	}
	if (rd->side == 0)
		return (rd->sideDistX - rd->deltaDistX);
	else
		return (rd->sideDistY - rd->deltaDistY);
}