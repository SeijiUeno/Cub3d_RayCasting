#include "../includes/app.h"
#include "../includes/parsing.h"
#include "../includes/render.h"
#include "../includes/input_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <MLX42/MLX42.h>

#define WIDTH 1280
#define HEIGHT 720

/* Cleanup function to free allocated memory */
void	cleanup_app(t_app *app)
{
	if (app->map.world)
	{
		for (int i = 0; i < app->map.height; i++)
			free(app->map.world[i]);
		free(app->map.world);
	}
	free(app->config.texNorthPath);
	free(app->config.texSouthPath);
	free(app->config.texEastPath);
	free(app->config.texWestPath);
}

int	main(int argc, char **argv)
{
	t_app	app;

	if (argc != 2)
	{
		printf("Usage: %s file.cub\n", argv[0]);
		return (EXIT_FAILURE);
	}
	/* Initialize defaults */
	app.player.moveSpeed = 0.05;
	app.player.rotSpeed = 0.03;
	app.config.floorColor = 0x006400FF;
	app.config.ceilingColor = 0x87CEEBFF;
	app.map.world = NULL;
	/* Parse the configuration and map */
	parse_cub_file(&app, argv[1]);
	app.gfx.mlx = mlx_init(WIDTH, HEIGHT, "Raycaster", true);
	if (!app.gfx.mlx)
	{
		printf("MLX init error\n");
		return (EXIT_FAILURE);
	}
	app.gfx.texNorth = mlx_load_png(app.config.texNorthPath);
	app.gfx.texSouth = mlx_load_png(app.config.texSouthPath);
	app.gfx.texEast  = mlx_load_png(app.config.texEastPath);
	app.gfx.texWest  = mlx_load_png(app.config.texWestPath);
	if (!app.gfx.texNorth || !app.gfx.texSouth || !app.gfx.texEast || !app.gfx.texWest)
	{
		printf("Error: One or more textures failed to load.\n");
		return (EXIT_FAILURE);
	}
	app.gfx.img = mlx_new_image(app.gfx.mlx, WIDTH, HEIGHT);
	if (!app.gfx.img)
	{
		mlx_close_window(app.gfx.mlx);
		return (EXIT_FAILURE);
	}
	mlx_image_to_window(app.gfx.mlx, app.gfx.img, 0, 0);
	mlx_loop_hook(app.gfx.mlx, update_frame, &app);
	mlx_loop(app.gfx.mlx);
	mlx_terminate(app.gfx.mlx);
	cleanup_app(&app);
	return (EXIT_SUCCESS);
}
