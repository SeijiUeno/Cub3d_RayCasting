/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:41:42 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:22:36 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/app.h"
#include "../includes/parsing.h"
#include "../includes/render.h"

static void	cleanup_app(t_app *app)
{
	int	i;

	i = 0;
	if (app->map.world)
	{
		while (i < app->map.height)
		{
			if ((app->map.world)[i])
				free(app->map.world[i]);
			i++;
		}
		if (app->map.world)
			free(app->map.world);
	}
	if (app->config.texNorthPath)
		free(app->config.texNorthPath);
	if (app->config.texSouthPath)
		free(app->config.texSouthPath);
	if (app->config.texEastPath)
		free(app->config.texEastPath);
	if (app->config.texWestPath)
		free(app->config.texWestPath);
}

static void	init_mlx_and_textures(t_app *app)
{
	app->gfx.mlx = mlx_init(WIDTH, HEIGHT, "Raycaster", true);
	if (!app->gfx.mlx)
	{
		exit(EXIT_FAILURE);
	}
	app->gfx.texNorth = mlx_load_png(app->config.texNorthPath);
	app->gfx.texSouth = mlx_load_png(app->config.texSouthPath);
	app->gfx.texEast = mlx_load_png(app->config.texEastPath);
	app->gfx.texWest = mlx_load_png(app->config.texWestPath);
	if (!app->gfx.texNorth || !app->gfx.texSouth
		|| !app->gfx.texEast || !app->gfx.texWest)
	{
		cleanup_app(app);
		exit(EXIT_FAILURE);
	}
	app->gfx.img = mlx_new_image(app->gfx.mlx, WIDTH, HEIGHT);
	if (!app->gfx.img)
	{
		mlx_close_window(app->gfx.mlx);
		cleanup_app(app);
		exit(EXIT_FAILURE);
	}
	mlx_image_to_window(app->gfx.mlx, app->gfx.img, 0, 0);
}

static void	init_app(t_app *app, char *filename)
{
	app->player.moveSpeed = 0.05;
	app->player.rotSpeed = 0.03;
	app->config.floorColor = 0x006400FF;
	app->config.ceilingColor = 0x87CEEBFF;
	app->map.world = NULL;
	parse_cub_file(app, filename);
}

static void	run_app(t_app *app)
{
	init_mlx_and_textures(app);
	mlx_loop_hook(app->gfx.mlx, update_frame, app);
	mlx_loop(app->gfx.mlx);
	mlx_terminate(app->gfx.mlx);
	cleanup_app(app);
}

int	main(int argc, char **argv)
{
	t_app	app;

	if (argc != 2)
	{
		printf("Usage: %s file.cub\n", argv[0]);
		return (EXIT_FAILURE);
	}
	init_app(&app, argv[1]);
	run_app(&app);
	return (EXIT_SUCCESS);
}
