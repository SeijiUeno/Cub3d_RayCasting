#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <stdlib.h>
#include <math.h>

/* draw_wall: Draws the wall slice.
   Parameters: (t_app *app, int x, t_column_draw *cd, t_tex_info tex)
*/
// ABGR to RGBA
static uint32_t	convert_texture_color(uint32_t color)
{
	uint32_t	a;
	uint32_t	b;
	uint32_t	g;
	uint32_t	r;

	a = (color >> 24) & 0xff;
	b = (color >> 16) & 0xff;
	g = (color >> 8) & 0xff;
	r = color & 0xff;
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

static	uint32_t	get_tex_color(const t_tex_info tex, int texY)
{
	uint32_t	*pixels;
	uint32_t	color;

	pixels = (uint32_t *)tex.texture->pixels;
	color = pixels[texY * tex.texWidth + tex.texX];
	return (convert_texture_color(color));
}

void	draw_wall(t_app *app, int x, t_column_draw *cd, t_tex_info tex)
{
	int			y;
	int			d;
	int			texY;
	uint32_t	wallColor;

	y = cd->drawStart;
	while (y <= cd->drawEnd)
	{
		d = y * 256 - HEIGHT * 128 + cd->lineHeight * 128;
		texY = ((d * tex.texHeight) / cd->lineHeight) / 256;
		if (texY < 0)
			texY = 0;
		if (texY >= tex.texHeight)
			texY = tex.texHeight - 1;
		wallColor = get_tex_color(tex, texY);
		mlx_put_pixel(app->gfx.img, x, y, wallColor);
		y = y + 1;
	}
}