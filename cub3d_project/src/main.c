/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:41:42 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:55:33 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/app.h"
#include "../includes/parsing.h"
#include "../includes/render.h"
#include "convert/cub3d.h"
#include "convert/convert.h"

static void	init_mlx_and_textures(t_app *app)
{
	app->gfx.mlx = mlx_init(WIDTH, HEIGHT, "Raycaster", true);
	if (!app->gfx.mlx)
	{
		exit(EXIT_FAILURE);
	}
	app->gfx.tex_no = mlx_load_png(app->config.tex_no_ph);
	app->gfx.tex_so = mlx_load_png(app->config.tex_so_ph);
	app->gfx.tex_ea = mlx_load_png(app->config.tex_ea_ph);
	app->gfx.tex_we = mlx_load_png(app->config.tex_we_ph);
	if (!app->gfx.tex_no || !app->gfx.tex_so
		|| !app->gfx.tex_ea || !app->gfx.tex_we)
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

static void	init_app(t_app *app, t_file *cube)
{
	app->player.move_sped = 0.05;
	app->player.rot_sped = 0.03;
	app->config.floor_color = 0x006400FF;
	app->config.sky_color = 0x87CEEBFF;
	app->map.world = NULL;
	parse_cub_data(app, cube);
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
	t_file	cub;

	if (argc != 2)
	{
		printf("Usage: %s file.cub\n", argv[0]);
		return (EXIT_FAILURE);
	}
	memset(&cub, 0, sizeof(t_file));
	convert(argc, argv, &cub.level);
	init_app(&app, &cub);
	run_app(&app);
	return (EXIT_SUCCESS);
}
