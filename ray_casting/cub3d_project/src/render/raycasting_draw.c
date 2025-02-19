/* src/raycasting_draw.c */
#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

t_tex_info select_tex_info(t_app *app, t_ray_data *rd, double wallX)
{
	t_tex_info info;
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
	if ((rd->side == 0 && rd->rayDirX > 0) ||
	    (rd->side == 1 && rd->rayDirY < 0))
		info.texX = info.texWidth - info.texX - 1;
	return info;
}

/* draw_wall: Draws the wall slice.
   Parameters: (t_app *app, int x, t_column_draw *cd, t_tex_info tex)
*/
void draw_wall(t_app *app, int x, t_column_draw *cd, t_tex_info tex)
{
	int y = cd->drawStart;
	while (y <= cd->drawEnd)
	{
		int d = y * 256 - HEIGHT * 128 + cd->lineHeight * 128;
		int texY = ((d * tex.texHeight) / cd->lineHeight) / 256;
		if (texY < 0)
			texY = 0;
		if (texY >= tex.texHeight)
			texY = tex.texHeight - 1;
		{
			uint32_t wallColor = ((uint32_t *)tex.texture->pixels)[texY * tex.texWidth + tex.texX];
			mlx_put_pixel(app->gfx.img, x, y, wallColor);
		}
		y = y + 1;
	}
}

/* draw_column_data: Draws ceiling, wall, and floor.
   Parameters: (t_app *app, t_column_draw *cd, t_tex_info tex)
*/
void draw_column_data(t_app *app, t_column_draw *cd, t_tex_info tex)
{
	int y = 0;
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
void render_column(t_app *app, int x)
{
	t_ray_data rd;
	t_column_draw cd;
	init_ray_data(app, x, &rd);
	/* Use init_ray_data to fill rd; then compute perpendicular distance */
	double perpWallDist = dda(app, &rd);
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
	cd.side = rd.side;
	if (rd.side == 0)
		cd.wallX = app->player.posY + perpWallDist * rd.rayDirY;
	else
		cd.wallX = app->player.posX + perpWallDist * rd.rayDirX;
	cd.wallX = cd.wallX - floor(cd.wallX);
	cd.rayDirX = rd.rayDirX;
	cd.rayDirY = rd.rayDirY;
	{
		t_tex_info tex = select_tex_info(app, &rd, cd.wallX);
		draw_column_data(app, &cd, tex);
	}
}

/* render_3d_view: Renders the 3D scene column by column.
   Parameters: (t_app *app)
*/
void render_3d_view(t_app *app)
{
	int x = 0;
	while (x < WIDTH)
	{
		render_column(app, x);
		x = x + 1;
	}
}