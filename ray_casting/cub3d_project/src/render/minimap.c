/* src/minimap.c */
#include "../../includes/render.h"
#include "../../includes/utils.h"

/* draw_map_cell: Draws one map cell at (mx, my) */
void draw_map_cell(t_app *app, int mx, int my, uint32_t color)
{
	int y = 0;
	while (y < MINIMAP_SCALE)
	{
		int x = 0;
		while (x < MINIMAP_SCALE)
		{
			int px = mx * MINIMAP_SCALE + x;
			int py = my * MINIMAP_SCALE + y;
			if (px >= 0 && px < (int)app->gfx.img->width &&
			    py >= 0 && py < (int)app->gfx.img->height)
				mlx_put_pixel(app->gfx.img, px, py, color);
			x = x + 1;
		}
		y = y + 1;
	}
}

/* draw_map_grid: Iterates over the map and draws each cell */
void draw_map_grid(t_app *app)
{
	int my = 0;
	while (my < app->map.height)
	{
		int mx = 0;
		while (mx < app->map.width)
		{
			uint32_t color;
			if (app->map.world[my][mx] > 0)
				color = ft_pixel(0, 0, 0, 255);
			else
				color = ft_pixel(255, 255, 255, 255);
			draw_map_cell(app, mx, my, color);
			mx = mx + 1;
		}
		my = my + 1;
	}
}

/* draw_player_minimap: Draws the player's icon and direction on the minimap */
void draw_player_minimap(t_app *app)
{
	int playerMiniX = (int)(app->player.posX * MINIMAP_SCALE);
	int playerMiniY = (int)(app->player.posY * MINIMAP_SCALE);
	int dy = -1;
	while (dy <= 1)
	{
		int dx = -1;
		while (dx <= 1)
		{
			int px = playerMiniX + dx;
			int py = playerMiniY + dy;
			if (px >= 0 && px < (int)app->gfx.img->width &&
			    py >= 0 && py < (int)app->gfx.img->height)
				mlx_put_pixel(app->gfx.img, px, py, ft_pixel(255, 0, 0, 255));
			dx = dx + 1;
		}
		dy = dy + 1;
	}
	{
		int endX = playerMiniX + (int)(app->player.dirX * 8);
		int endY = playerMiniY + (int)(app->player.dirY * 8);
		draw_line(app->gfx.img, playerMiniX, playerMiniY, endX, endY,
			ft_pixel(0, 0, 255, 255));
	}
}

/* render_minimap: Renders the map grid and the player's icon */
void render_minimap(t_app *app)
{
	draw_map_grid(app);
	draw_player_minimap(app);
}
