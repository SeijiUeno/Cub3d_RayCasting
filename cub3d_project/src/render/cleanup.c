/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:30:43 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:30:44 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void	cleanup_textures(t_app *app)
{
	if (app->gfx.tex_so)
		mlx_delete_texture(app->gfx.tex_so);
	if (app->gfx.tex_no)
		mlx_delete_texture(app->gfx.tex_no);
	if (app->gfx.tex_ea)
		mlx_delete_texture(app->gfx.tex_ea);
	if (app->gfx.tex_we)
		mlx_delete_texture(app->gfx.tex_we);
}

void	cleanup_app(t_app *app)
{
	size_t	i;

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
	cleanup_textures(app);
	if (app->config.tex_no_ph)
		free(app->config.tex_no_ph);
	if (app->config.tex_so_ph)
		free(app->config.tex_so_ph);
	if (app->config.tex_ea_ph)
		free(app->config.tex_ea_ph);
	if (app->config.tex_we_ph)
		free(app->config.tex_we_ph);
}
