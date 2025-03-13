/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:44:48 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/file_utils.h"
#include "../../includes/utils.h"
#include "../import/cub3d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void parse_cub_data(t_app *app, t_cub *cube)
{
    const char *prefix = "assets/textures/";

    /* Process texture paths.
     * If the texture path already begins with the prefix,
     * duplicate it. Otherwise, prepend the prefix.
     */
    if (ft_strncmp(cube->level.no, prefix, ft_strlen(prefix)) == 0)
        app->config.texNorthPath = ft_strdup(cube->level.no);
    else
        app->config.texNorthPath = prepend_prefix(prefix, cube->level.no);

    if (ft_strncmp(cube->level.so, prefix, ft_strlen(prefix)) == 0)
        app->config.texSouthPath = ft_strdup(cube->level.so);
    else
        app->config.texSouthPath = prepend_prefix(prefix, cube->level.so);

    if (ft_strncmp(cube->level.ea, prefix, ft_strlen(prefix)) == 0)
        app->config.texEastPath = ft_strdup(cube->level.ea);
    else
        app->config.texEastPath = prepend_prefix(prefix, cube->level.ea);

    if (ft_strncmp(cube->level.we, prefix, ft_strlen(prefix)) == 0)
        app->config.texWestPath = ft_strdup(cube->level.we);
    else
        app->config.texWestPath = prepend_prefix(prefix, cube->level.we);

    /* Process colors.
     * Convert the RGB arrays to a pixel value.
     */
    app->config.floorColor = ft_pixel(cube->level.f[0],
                                      cube->level.f[1],
                                      cube->level.f[2], 255);
    app->config.ceilingColor = ft_pixel(cube->level.c[0],
                                        cube->level.c[1],
                                        cube->level.c[2], 255);

    /* Process map data.
     * The map dimensions are stored as size_t, so our loops use size_t as well.
     */
    app->map.width = cube->level.x_size;
    app->map.height = cube->level.y_size;

    /* Allocate a 2D array for the map.
     * Each row is allocated as an array of int with size app->map.width.
     */
    app->map.world = malloc(sizeof(int *) * app->map.height);
    if (!app->map.world)
    {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < app->map.height; i++)
    {
        app->map.world[i] = malloc(sizeof(int) * app->map.width);
        if (!app->map.world[i])
        {
            perror("malloc error");
            exit(EXIT_FAILURE);
        }
        for (size_t j = 0; j < app->map.width; j++)
        {
            char c = cube->level.map[i][j];
            if (c >= '0' && c <= '9')
                app->map.world[i][j] = c - '0';
            else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                /* Set player starting position */
                app->player.pos_x = j + 0.5;
                app->player.pos_y = i + 0.5;
                if (c == 'N')
                {
                    app->player.dir_x = 0;
                    app->player.dir_y = -1;
                    app->player.planeX = 0.66;
                    app->player.planeY = 0;
                }
                else if (c == 'S')
                {
                    app->player.dir_x = 0;
                    app->player.dir_y = 1;
                    app->player.planeX = -0.66;
                    app->player.planeY = 0;
                }
                else if (c == 'E')
                {
                    app->player.dir_x = 1;
                    app->player.dir_y = 0;
                    app->player.planeX = 0;
                    app->player.planeY = 0.66;
                }
                else if (c == 'W')
                {
                    app->player.dir_x = -1;
                    app->player.dir_y = 0;
                    app->player.planeX = 0;
                    app->player.planeY = -0.66;
                }
                /* Mark player's starting cell as empty */
                app->map.world[i][j] = 0;
            }
            else if (c == ' ' || c == '\0')
            {
                app->map.world[i][j] = 0;
            }
            else
            {
                fprintf(stderr, "Invalid map character '%c' at (%zu, %zu)\n", c, i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
}
