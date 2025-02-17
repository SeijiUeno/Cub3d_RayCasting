/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 10:57:15 by sueno-te          #+#    #+#             */
/*   Updated: 2025/02/17 11:37:45 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH       512
#define HEIGHT      512
#define MAP_WIDTH   5
#define MAP_HEIGHT  5
#define PLAYER_SIZE 16

// The map: 1 = wall (black), 0 = floor (white)
static int world_map[MAP_WIDTH * MAP_HEIGHT] = {
    1,1,1,1,1,
    1,0,0,0,1,
    1,0,1,0,1,
    1,0,0,0,1,
    1,1,1,1,1,
};

static mlx_image_t* map_img;
static mlx_image_t* player_img;
// We'll store the player's instance index once added to the window.
int player_index = 0;

// The size of each tile; since the map covers the full screen, we use:
static const int tile_size = WIDTH / MAP_WIDTH; // 512/5 = 102

// Helper: Pack an RGBA color into a 32-bit integer.
static inline uint32_t ft_pixel(int r, int g, int b, int a)
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

// Draw the 5x5 map onto the map image.
void draw_map(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int tile = world_map[y * MAP_WIDTH + x];
            uint32_t color = (tile == 1)
                ? ft_pixel(0, 0, 0, 255)         // Black for walls
                : ft_pixel(255, 255, 255, 255);    // White for floor

            // Fill the tile area with the chosen color.
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

// Check all four corners of the player's square for collision.
bool collides(void)
{
    int px = player_img->instances[player_index].x;
    int py = player_img->instances[player_index].y;
    int right = px + PLAYER_SIZE - 1;
    int bottom = py + PLAYER_SIZE - 1;

    // Define the four corners.
    int corners[4][2] = {
        {px, py},           // top-left
        {right, py},        // top-right
        {px, bottom},       // bottom-left
        {right, bottom}     // bottom-right
    };

    for (int i = 0; i < 4; i++)
    {
        int corner_x = corners[i][0];
        int corner_y = corners[i][1];

        int tile_x = corner_x / tile_size;
        int tile_y = corner_y / tile_size;

        // If the corner is out-of-bounds or in a wall tile, we have a collision.
        if (tile_x < 0 || tile_x >= MAP_WIDTH ||
            tile_y < 0 || tile_y >= MAP_HEIGHT ||
            world_map[tile_y * MAP_WIDTH + tile_x] == 1)
        {
            return true;
        }
    }
    return false;
}

// Handle input and movement with collision detection.
void handle_input(void* param)
{
    mlx_t* mlx = (mlx_t*)param;

    // Save the current position.
    int old_x = player_img->instances[player_index].x;
    int old_y = player_img->instances[player_index].y;

    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    // Update position based on arrow keys.
    if (mlx_is_key_down(mlx, MLX_KEY_UP))
        player_img->instances[player_index].y -= 2;
    if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
        player_img->instances[player_index].y += 2;
    if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
        player_img->instances[player_index].x -= 2;
    if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
        player_img->instances[player_index].x += 2;

    // If any of the four corners is colliding, revert to the old position.
    if (collides())
    {
        player_img->instances[player_index].x = old_x;
        player_img->instances[player_index].y = old_y;
    }
}

int main(void)
{
    mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "Cub3D with Full Collision", true);
    if (!mlx)
    {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    // Create the map image (full screen).
    map_img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!map_img)
    {
        puts("Error creating map image");
        mlx_terminate(mlx);
        return EXIT_FAILURE;
    }

    // Create the player image (16x16 red square).
    player_img = mlx_new_image(mlx, PLAYER_SIZE, PLAYER_SIZE);
    if (!player_img)
    {
        puts("Error creating player image");
        mlx_terminate(mlx);
        return EXIT_FAILURE;
    }
    // Fill the player image with red.
    for (uint32_t i = 0; i < player_img->width; i++)
    {
        for (uint32_t j = 0; j < player_img->height; j++)
            mlx_put_pixel(player_img, i, j, ft_pixel(255, 0, 0, 255));
    }

    // Draw the map once.
    draw_map();

    // Add the map image to the window.
    mlx_image_to_window(mlx, map_img, 0, 0);

    // Add the player image to the window.
    // The returned value is the instance index that we store for later use.
    // We position the player in tile (1,1) centered in that tile.
    player_index = mlx_image_to_window(mlx, player_img,
                           1 * tile_size + (tile_size - PLAYER_SIZE) / 2,
                           1 * tile_size + (tile_size - PLAYER_SIZE) / 2);

    // Set up the input handler.
    mlx_loop_hook(mlx, handle_input, mlx);

    // Start the main loop.
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}