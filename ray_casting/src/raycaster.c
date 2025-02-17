/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 09:39:37 by sueno-te          #+#    #+#             */
/*   Updated: 2025/02/17 09:50:48 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH       512
#define HEIGHT      512
#define MAP_WIDTH   5
#define MAP_HEIGHT  5

// 1 = wall (black), 0 = floor (white)
static int world_map[MAP_WIDTH * MAP_HEIGHT] = {
    1,1,1,1,1,
    1,0,0,0,1,
    1,0,1,0,1,
    1,0,0,0,1,
    1,1,1,1,1,
};

static mlx_image_t* map_img;
static mlx_image_t* player_img;

// Helper to pack an RGBA color
static inline uint32_t ft_pixel(int r, int g, int b, int a)
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

// Draw the entire map onto map_img, filling up the window
void draw_map(void)
{
    // Each cell is tile_size×tile_size
    int tile_size = WIDTH / MAP_WIDTH;  // (512 / 5) = 102

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // Decide color based on tile
            int tile = world_map[y * MAP_WIDTH + x];
            uint32_t color = (tile == 1)
                ? ft_pixel(0, 0, 0, 255)         // black for wall
                : ft_pixel(255, 255, 255, 255);  // white for floor

            // Fill a TILE_SIZE × TILE_SIZE square
            for (int py = 0; py < tile_size; py++)
            {
                for (int px = 0; px < tile_size; px++)
                {
                    int pixel_x = x * tile_size + px;
                    int pixel_y = y * tile_size + py;
                    if ((uint32_t)pixel_x < map_img->width && (uint32_t)pixel_y < map_img->height)
                        mlx_put_pixel(map_img, pixel_x, pixel_y, color);
                }
            }
        }
    }
}

// Handle movement or close
void handle_input(void* param)
{
    mlx_t* mlx = (mlx_t*)param;
    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    // Simple arrow key movement
    if (mlx_is_key_down(mlx, MLX_KEY_UP))
        player_img->instances[0].y -= 2;
    if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
        player_img->instances[0].y += 2;
    if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
        player_img->instances[0].x -= 2;
    if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
        player_img->instances[0].x += 2;
}

int main(void)
{
    // 1) Create the main window (512×512)
    mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "Cub3D - Fullscreen Map", true);
    if (!mlx)
    {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    // 2) Create a map image sized exactly like the window
    map_img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!map_img)
    {
        puts("Error creating map image");
        mlx_terminate(mlx);
        return EXIT_FAILURE;
    }

    // 3) Create a player image (16×16 red square)
    player_img = mlx_new_image(mlx, 16, 16);
    if (!player_img)
    {
        puts("Error creating player image");
        mlx_terminate(mlx);
        return EXIT_FAILURE;
    }
    // Fill the player image with red
    for (uint32_t i = 0; i < player_img->width; i++)
    {
        for (uint32_t j = 0; j < player_img->height; j++)
            mlx_put_pixel(player_img, i, j, ft_pixel(255, 0, 0, 255));
    }

    // 4) Draw the map once so it covers the whole window
    draw_map();

    // 5) Add the images to the window
    //    The map is the background, then the player on top
    mlx_image_to_window(mlx, map_img, 0, 0);
    // Place player in the center of the second cell
    mlx_image_to_window(mlx, player_img, 102 + 8, 102 + 8);

    // 6) Hook for user input (moving the player, closing the window)
    mlx_loop_hook(mlx, handle_input, mlx);

    // 7) Start the loop
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}