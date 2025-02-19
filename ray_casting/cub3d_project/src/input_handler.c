#include "../includes/input_handler.h"
#include <MLX42/MLX42.h>
#include <math.h>

void	rotate_player(t_app *app, double angle)
{
	double	cosA = cos(angle);
	double	sinA = sin(angle);
	double	oldDirX = app->player.dirX;

	app->player.dirX = app->player.dirX * cosA - app->player.dirY * sinA;
	app->player.dirY = oldDirX * sinA + app->player.dirY * cosA;
	double	oldPlaneX = app->player.planeX;
	app->player.planeX = app->player.planeX * cosA - app->player.planeY * sinA;
	app->player.planeY = oldPlaneX * sinA + app->player.planeY * cosA;
	{
		double len = sqrt(app->player.dirX * app->player.dirX +
			app->player.dirY * app->player.dirY);
		if (len != 0)
		{
			app->player.dirX /= len;
			app->player.dirY /= len;
		}
	}
	{
		double len = sqrt(app->player.planeX * app->player.planeX +
			app->player.planeY * app->player.planeY);
		if (len != 0)
		{
			app->player.planeX = (app->player.planeX / len) * 0.66;
			app->player.planeY = (app->player.planeY / len) * 0.66;
		}
	}
}

void	process_input(t_app *app)
{
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_ESCAPE))
		mlx_close_window(app->gfx.mlx);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_W))
	{
		double newX = app->player.posX + app->player.dirX * app->player.moveSpeed;
		double newY = app->player.posY + app->player.dirY * app->player.moveSpeed;
		if (newX >= 0 && newX < app->map.width &&
			app->map.world[(int)app->player.posY][(int)newX] == 0)
			app->player.posX = newX;
		if (newY >= 0 && newY < app->map.height &&
			app->map.world[(int)newY][(int)app->player.posX] == 0)
			app->player.posY = newY;
	}
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_S))
	{
		double newX = app->player.posX - app->player.dirX * app->player.moveSpeed;
		double newY = app->player.posY - app->player.dirY * app->player.moveSpeed;
		if (newX >= 0 && newX < app->map.width &&
			app->map.world[(int)app->player.posY][(int)newX] == 0)
			app->player.posX = newX;
		if (newY >= 0 && newY < app->map.height &&
			app->map.world[(int)newY][(int)app->player.posX] == 0)
			app->player.posY = newY;
	}
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_A))
		rotate_player(app, app->player.rotSpeed);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_D))
		rotate_player(app, -app->player.rotSpeed);
}
