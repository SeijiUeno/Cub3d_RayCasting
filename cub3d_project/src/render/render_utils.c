/* src/render_utils.c */
#include "../../includes/render.h"
#include <stdlib.h>

/* clear_image: Clears an image by setting every pixel to a color */
void clear_image(mlx_image_t *img, uint32_t color)
{
	size_t total = (size_t)img->width * img->height;
	size_t i = 0;
	uint32_t *pixels = (uint32_t *)img->pixels;
	while (i < total)
	{
		pixels[i] = color;
		i = i + 1;
	}
}

/* draw_line: Draws a line from (x0,y0) to (x1,y1) using Bresenham's algorithm */
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx, dy, sx, sy, err, e2;
	if (x1 >= x0) { dx = x1 - x0; sx = 1; }
	else { dx = x0 - x1; sx = -1; }
	if (y1 >= y0) { dy = y1 - y0; sy = 1; }
	else { dy = y0 - y1; sy = -1; }
	err = dx - dy;
	while (1)
	{
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
			mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err * 2;
		if (e2 > -dy) { err = err - dy; x0 = x0 + sx; }
		if (e2 < dx) { err = err + dx; y0 = y0 + sy; }
	}
}
