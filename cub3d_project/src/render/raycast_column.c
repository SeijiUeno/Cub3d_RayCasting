#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <math.h>
#include <stdlib.h>

/* draw_column_data: Draws ceiling, wall, and floor.
   Parameters: (t_app *app, t_column_draw *cd, t_tex_info tex)
*/
void	draw_column_data(t_app *app, t_column_draw *cd, t_tex_info tex)
{
	int	y;

	y = 0;
	while (y < cd->drawStart)
	{
		mlx_put_pixel(app->gfx.img, cd->x, y, app->config.ceilingColor);
		y = y + 1;
	}
	draw_wall(app, cd->x, cd, tex);
	y = cd->drawEnd + 1;
	while (y < HEIGHT)
	{
		mlx_put_pixel(app->gfx.img, cd->x, y, app->config.floorColor);
		y = y + 1;
	}
}

/* render_column: Computes data for one column and draws it.
   Parameters: (t_app *app, int x)
*/
static void	process_column_draw(t_app *app, t_ray_data rd,
	t_column_draw *cd, double perpWallDist)
{
	t_tex_info	tex;

	if (rd.side == 0)
	{
		cd->wallX = app->player.posY + perpWallDist * rd.rayDirY;
	}
	else
	{
		cd->wallX = app->player.posX + perpWallDist * rd.rayDirX;
	}
	cd->wallX = cd->wallX - floor(cd->wallX);
	cd->rayDirX = rd.rayDirX;
	cd->rayDirY = rd.rayDirY;
	tex = select_tex_info(app, &rd, cd->wallX);
	draw_column_data(app, cd, tex);
}

void	render_column(t_app *app, int x)
{
	t_ray_data		d;
	t_column_draw	cd;
	double			perpWallDist;

	init_ray_data(app, x, &d);
	perpWallDist = dda(app, &d);
	if (perpWallDist < 0.1)
		perpWallDist = 0.1;
	cd.lineHeight = (int)(HEIGHT / perpWallDist);
	if (cd.lineHeight <= 0)
		cd.lineHeight = 1;
	cd.drawStart = -cd.lineHeight / 2 + HEIGHT / 2;
	if (cd.drawStart < 0)
		cd.drawStart = 0;
	cd.drawEnd = cd.lineHeight / 2 + HEIGHT / 2;
	if (cd.drawEnd >= HEIGHT)
		cd.drawEnd = HEIGHT - 1;
	cd.x = x;
	cd.side = d.side;
	process_column_draw(app, d, &cd, perpWallDist);
}
