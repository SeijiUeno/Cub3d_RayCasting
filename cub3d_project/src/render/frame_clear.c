#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"
#include <stdlib.h>

void	clear_image(mlx_image_t *img, uint32_t color)
{
	size_t		total;
	size_t		i;
	uint32_t	*pixels;

	total = (size_t)img->width * img->height;
	i = 0;
	pixels = (uint32_t *)img->pixels;
	while (i < total)
	{
		pixels[i] = color;
		i = i + 1;
	}
}