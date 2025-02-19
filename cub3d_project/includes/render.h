#ifndef RENDER_H
# define RENDER_H

# include "app.h"

# define WIDTH 1280
# define HEIGHT 720
# define CLEAR_COLOR 0x000000FF
# define MINIMAP_SCALE 6

void	clear_image(mlx_image_t *img, uint32_t color);
void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color);
void	render_3d_view(t_app *app);
void	render_minimap(t_app *app);
void	update_frame(void *param);

#endif
