/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 09:47:27 by sueno-te          #+#    #+#             */
/*   Updated: 2025/02/17 10:56:18 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTER_H
# define RAYCASTER_H

#ifndef CUB3D_H
# define CUB3D_H

# include <MLX42/MLX42.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# define WIDTH 512
# define HEIGHT 512
# define MAP_WIDTH 5
# define MAP_HEIGHT 5
# define PLAYER_SIZE 16
# define TILE_SIZE (WIDTH / MAP_WIDTH)

typedef struct s_player
{
    double      x;
    double      y;
    double      dir_x;
    double      dir_y;
    double      plane_x;
    double      plane_y;
    float       move_speed;
    float       rot_speed;
}               t_player;

typedef struct s_map
{
    int         width;
    int         height;
    int         *data;
}               t_map;

typedef struct s_game
{
    mlx_t       *mlx;
    mlx_image_t *map_img;
    mlx_image_t *player_img;
    mlx_image_t *view3d;
    t_player    player;
    t_map       map;
}               t_game;

// Helper functions
uint32_t    ft_pixel(int r, int g, int b, int a);

// Initialization
void        init_game(t_game *game);
void        init_map(t_game *game);
void        init_player(t_game *game);

// Rendering
void        draw_map(t_game *game);
void        render_player(t_game *game);

// Input
void        handle_input(void *param);

// Physics
bool        collides(t_game *game, double new_x, double new_y);

#endif


#endif