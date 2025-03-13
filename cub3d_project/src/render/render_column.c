#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <math.h>
#include <stdlib.h>

t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wall_x)
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
	info.texX = (int)(wall_x * info.texWidth);
	if ((rd->side == 0 && rd->rayDirX > 0)
		|| (rd->side == 1 && rd->rayDirY < 0))
		info.texX = info.texWidth - info.texX - 1;
	return (info);
}

/*
** Pure function that calculates the vertical limits for a wall slice.
** It computes the lineHeight as HEIGHT / perpWallDist, and then gives
** the starting and ending y coordinates (clamped appropriately).
*/

static void	compute_wall_slice_limits(t_column_draw *col_draw,
	double perp_wall_dist)
{
	col_draw->lineHeight = (int)(HEIGHT / perp_wall_dist);
	if (col_draw->lineHeight <= 0)
		col_draw->lineHeight = 1;

	col_draw->drawStart = -col_draw->lineHeight / 2 + HEIGHT / 2;
	if (col_draw->drawStart < 0)
		col_draw->drawStart = 0;
	col_draw->drawEnd = col_draw->lineHeight / 2 + HEIGHT / 2;
	if (col_draw->drawEnd >= HEIGHT)
		col_draw->drawEnd = HEIGHT - 1;
}

/*
Based on the computed ray data, determine the x-coordinate
(wallHitX) where the ray intersects the wall, normalize it for texture mapping,
then select the correct texture and draw the full column.
*/
static void	map_wall_texture_coordinates(t_app *app, t_ray_data rd,
	t_column_draw *col_draw, double perp_wall_dist)
{
	t_tex_info	tex;

	if (rd.side == 0)
	{
		col_draw->wallX = app->player.posY + perp_wall_dist * rd.rayDirY;
	}
	else
	{
		col_draw->wallX = app->player.posX + perp_wall_dist * rd.rayDirX;
	}
	col_draw->wallX = col_draw->wallX - floor(col_draw->wallX);
	col_draw->rayDirX = rd.rayDirX;
	col_draw->rayDirY = rd.rayDirY;
	tex = select_tex_info(app, &rd, col_draw->wallX);
	draw_full_column(app, col_draw, tex);
}

/*
For the given screen x coordinate:
- Initialize ray data and calculate the perpendicular wall distance.
- Compute the vertical slice limits using compute_wall_slice_limits.
- Set the column index and side.
- Map the wall intersection to texture coordinates and draw the column.
*/
void	render_column(t_app *app, int x)
{
	t_ray_data		d;
	t_column_draw	col_draw;
	double			perp_wall_dist;

	init_ray_data(app, x, &d);
	perp_wall_dist = dda(app, &d);
	if (perp_wall_dist < 0.1)
		perp_wall_dist = 0.1;
	compute_wall_slice_limits(&col_draw, perp_wall_dist);
	col_draw.x = x;
	col_draw.side = d.side;
	map_wall_texture_coordinates(app, d, &col_draw, perp_wall_dist);
}
