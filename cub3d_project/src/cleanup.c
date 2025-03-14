
#include "../includes/render.h"
#include "../includes/structures.h"

static void	cleanup_textures(t_app *app)
{
	if (app->gfx.texNorth)
		mlx_delete_texture(app->gfx.texNorth);
	if (app->gfx.texSouth)
		mlx_delete_texture( app->gfx.texSouth);
	if (app->gfx.texEast)
		mlx_delete_texture(app->gfx.texEast);
	if (app->gfx.texWest)
		mlx_delete_texture(app->gfx.texWest);
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
	if (app->config.texNorthPath)
		free(app->config.texNorthPath);
	if (app->config.texSouthPath)
		free(app->config.texSouthPath);
	if (app->config.texEastPath)
		free(app->config.texEastPath);
	if (app->config.texWestPath)
		free(app->config.texWestPath);
}
