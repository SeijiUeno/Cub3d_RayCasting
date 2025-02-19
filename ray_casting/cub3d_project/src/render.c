#include "../includes/render.h"
#include "../includes/utils.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void	clear_image(mlx_image_t *img, uint32_t color)
{
	size_t	total = (size_t)img->width * img->height;
	uint32_t	*pixels = (uint32_t *)img->pixels;

	for (size_t i = 0; i < total; i++)
		pixels[i] = color;
}

void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int	dx = (x1 > x0) ? x1 - x0 : x0 - x1;
	int	dy = (y1 > y0) ? y1 - y0 : y0 - y1;
	int	sx = (x0 < x1) ? 1 : -1;
	int	sy = (y0 < y1) ? 1 : -1;
	int	err = dx - dy;
	int	e2;

	while (1)
	{
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
			mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

/* In this module we implement the raycasting view.
   The function uses the app state (player, config, gfx, map) */
void	render_3d_view(t_app *app)
{
	t_player	*p = &app->player;
	t_config	*cfg = &app->config;
	t_graphics	*gfx = &app->gfx;
	int			mapX, mapY;
	double		rayDirX, rayDirY, cameraX, deltaDistX, deltaDistY;
	double		sideDistX, sideDistY, perpWallDist;
	int			stepX, stepY;
	int			hit, side;
	int			lineHeight, drawStart, drawEnd;
	double		wallX;

	for (int x = 0; x < WIDTH; x++)
	{
		cameraX = 2 * x / (double)WIDTH - 1;
		rayDirX = p->dirX + p->planeX * cameraX;
		rayDirY = p->dirY + p->planeY * cameraX;
		mapX = (int)p->posX;
		mapY = (int)p->posY;
		deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (p->posX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - p->posX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (p->posY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - p->posY) * deltaDistY;
		}
		hit = 0;
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (mapX < 0 || mapX >= app->map.width || mapY < 0 || mapY >= app->map.height)
			{
				hit = 1;
				perpWallDist = HEIGHT;
				break;
			}
			if (app->map.world[mapY][mapX] > 0)
				hit = 1;
		}
		if (mapX >= 0 && mapX < app->map.width && mapY >= 0 && mapY < app->map.height)
			perpWallDist = (side == 0) ? sideDistX - deltaDistX : sideDistY - deltaDistY;
		if (perpWallDist < 0.2)
			perpWallDist = 0.2;
		lineHeight = (int)(HEIGHT / perpWallDist);
		if (lineHeight <= 0)
			lineHeight = 1;
		drawStart = -lineHeight / 2 + HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		drawEnd = lineHeight / 2 + HEIGHT / 2;
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;
		wallX = (side == 0) ? p->posY + perpWallDist * rayDirY : p->posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);
		mlx_texture_t *texture = (side == 0)
			? (rayDirX < 0 ? gfx->texEast : gfx->texWest)
			: (rayDirY < 0 ? gfx->texSouth : gfx->texNorth);
		int texWidth = texture->width;
		int texHeight = texture->height;
		int texX = (int)(wallX * texWidth);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
			texX = texWidth - texX - 1;
		for (int y = 0; y < HEIGHT; y++)
		{
			if (y < drawStart)
				mlx_put_pixel(gfx->img, x, y, cfg->ceilingColor);
			else if (y <= drawEnd)
			{
				int d = y * 256 - HEIGHT * 128 + lineHeight * 128;
				int texY = ((d * texHeight) / lineHeight) / 256;
				if (texY < 0)
					texY = 0;
				if (texY >= texHeight)
					texY = texHeight - 1;
				uint32_t wallColor = ((uint32_t *)texture->pixels)[texY * texWidth + texX];
				mlx_put_pixel(gfx->img, x, y, wallColor);
			}
			else
				mlx_put_pixel(gfx->img, x, y, cfg->floorColor);
		}
	}
}

void	render_minimap(t_app *app)
{
	t_graphics *gfx = &app->gfx;
	for (int my = 0; my < app->map.height; my++)
	{
		for (int mx = 0; mx < app->map.width; mx++)
		{
			uint32_t color = (app->map.world[my][mx] > 0)
				? ft_pixel(0, 0, 0, 255)
				: ft_pixel(255, 255, 255, 255);
			for (int y = 0; y < MINIMAP_SCALE; y++)
			{
				for (int x = 0; x < MINIMAP_SCALE; x++)
				{
					int px = mx * MINIMAP_SCALE + x;
					int py = my * MINIMAP_SCALE + y;
					if (px >= 0 && px < (int)gfx->img->width && py >= 0 && py < (int)gfx->img->height)
						mlx_put_pixel(gfx->img, px, py, color);
				}
			}
		}
	}
	int playerMiniX = (int)(app->player.posX * MINIMAP_SCALE);
	int playerMiniY = (int)(app->player.posY * MINIMAP_SCALE);
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			int px = playerMiniX + dx;
			int py = playerMiniY + dy;
			if (px >= 0 && px < (int)gfx->img->width && py >= 0 && py < (int)gfx->img->height)
				mlx_put_pixel(gfx->img, px, py, ft_pixel(255, 0, 0, 255));
		}
	}
	int endX = playerMiniX + (int)(app->player.dirX * 8);
	int endY = playerMiniY + (int)(app->player.dirY * 8);
	draw_line(gfx->img, playerMiniX, playerMiniY, endX, endY, ft_pixel(0, 0, 255, 255));
}

/* update_frame is called every frame by MLX */
void	update_frame(void *param)
{
	t_app	*app = (t_app *)param;
	/* Process input is in the input_handler module */
	extern void process_input(t_app *app);
	process_input(app);
	clear_image(app->gfx.img, CLEAR_COLOR);
	render_3d_view(app);
	render_minimap(app);
}
