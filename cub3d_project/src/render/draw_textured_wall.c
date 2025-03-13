#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <stdlib.h>
#include <math.h>

/*
Converts a 32-bit pixel from ABGR format
to RGBA format (as used in our rendering)
In ABGR the bytes are: [A][B][G][R]
while in RGBA they should be: [R][G][B][A]
*/
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

/*
get_tex_color:
Retrieves the texture color at the given vertical texture coordinate (tex_y)
and converts it to the proper format.
*/
static	uint32_t	get_tex_color(const t_tex_info tex, int tex_y)
{
	uint32_t	*pixels;
	uint32_t	texture_color;

	pixels = (uint32_t *)tex.texture->pixels;
	texture_color = pixels[tex_y * tex.texWidth + tex.texX];
	return (convert_texture_color(texture_color));
}

/*
compute_wall_pixel_color:
Computes the wall slice pixel color at screen coordinate y,
by determining the corresponding texture coordinate.
	d = y * 256 - screenHeight * 128 + lineHeight * 128;
	tex_y = ((d * tex.texHeight) / lineHeight) / 256;
It then returns the texture color at that tex_y.
*/

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

/*
Draws the wall slice for a given column (x) in the 3D view.
*/
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
