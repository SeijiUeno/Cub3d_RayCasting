#include "../includes/input_handler.h"
#include <MLX42/MLX42.h>
#include <math.h>

/* normalize_vector: Normalizes a 2D vector */
static void normalize_vector(double *x, double *y)
{
	double len = sqrt((*x) * (*x) + (*y) * (*y));
	if (len != 0)
	{
		*x /= len;
		*y /= len;
	}
}

/* rotate_player: Rotates the player's direction and camera plane */
void rotate_player(t_app *app, double angle)
{
	double cosA = cos(angle);
	double sinA = sin(angle);
	double oldDirX = app->player.dirX;
	app->player.dirX = app->player.dirX * cosA - app->player.dirY * sinA;
	app->player.dirY = oldDirX * sinA + app->player.dirY * cosA;
	double oldPlaneX = app->player.planeX;
	app->player.planeX = app->player.planeX * cosA - app->player.planeY * sinA;
	app->player.planeY = oldPlaneX * sinA + app->player.planeY * cosA;
	normalize_vector(&app->player.dirX, &app->player.dirY);
	normalize_vector(&app->player.planeX, &app->player.planeY);
	app->player.planeX *= 0.66;
	app->player.planeY *= 0.66;
}

/* move_player: Moves the player; mult = 1.0 for forward, -1.0 for backward */
static void move_player(t_app *app, double mult)
{
	double newX = app->player.posX + app->player.dirX * app->player.moveSpeed * mult;
	double newY = app->player.posY + app->player.dirY * app->player.moveSpeed * mult;
	if (newX >= 0 && newX < app->map.width &&
	    app->map.world[(int)app->player.posY][(int)newX] == 0)
		app->player.posX = newX;
	if (newY >= 0 && newY < app->map.height &&
	    app->map.world[(int)newY][(int)app->player.posX] == 0)
		app->player.posY = newY;
}

/* process_input: Checks key states and moves/rotates the player accordingly */
void process_input(t_app *app)
{
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_ESCAPE))
		mlx_close_window(app->gfx.mlx);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_W))
		move_player(app, 1.0);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_S))
		move_player(app, -1.0);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_D))
		rotate_player(app, app->player.rotSpeed);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_A))
		rotate_player(app, -app->player.rotSpeed);
}
