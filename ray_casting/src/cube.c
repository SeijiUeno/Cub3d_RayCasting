/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 09:39:37 by sueno-te          #+#    #+#             */
/*   Updated: 2025/02/17 11:53:08 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

// Screen dimensions
#define WIDTH 512
#define HEIGHT 512

// Map dimensions for raycasting
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Minimap scale (each map cell becomes a MINIMAP_SCALE x MINIMAP_SCALE square)
#define MINIMAP_SCALE 4

// --- World Map ---
// 1 = wall, 0 = empty space.
int worldMap[MAP_HEIGHT][MAP_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// --- Player variables ---
// Position in map units.
double posX = 10, posY = 20;
// Direction vector (normalized). Initially facing left.
double dirX = -1, dirY = 0;
// Camera plane (perpendicular to direction). Determines FOV.
double planeX = 0, planeY = 0.66;

// Global MLX42 objects.
mlx_t *mlx;
static mlx_image_t *img;

// --- Helper: Pack an RGBA color into a 32-bit integer.
static inline uint32_t ft_pixel(int r, int g, int b, int a)
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

// --- Draw a line using Bresenham's algorithm.
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (true)
    {
        if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
            mlx_put_pixel(img, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

// --- Draw the minimap in the upper left corner.
void draw_minimap(mlx_image_t *img)
{
    // Draw the map cells.
    for (int my = 0; my < MAP_HEIGHT; my++)
    {
        for (int mx = 0; mx < MAP_WIDTH; mx++)
        {
            uint32_t color;
            if (worldMap[my][mx] > 0)
                color = ft_pixel(0, 0, 0, 255);      // Wall: black.
            else
                color = ft_pixel(255, 255, 255, 255);  // Empty: white.

            // Fill a rectangle of size MINIMAP_SCALE x MINIMAP_SCALE.
            for (int y = 0; y < MINIMAP_SCALE; y++)
            {
                for (int x = 0; x < MINIMAP_SCALE; x++)
                {
                    int px = mx * MINIMAP_SCALE + x;
                    int py = my * MINIMAP_SCALE + y;
                    if (px < (int)img->width && py < (int)img->height)
                        mlx_put_pixel(img, px, py, color);
                }
            }
        }
    }

    // Draw the player on the minimap as a small red square.
    int playerMiniX = (int)(posX * MINIMAP_SCALE);
    int playerMiniY = (int)(posY * MINIMAP_SCALE);
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int px = playerMiniX + dx;
            int py = playerMiniY + dy;
            if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
                mlx_put_pixel(img, px, py, ft_pixel(255, 0, 0, 255));
        }
    }

    // Draw the player's POV line on the minimap in blue.
    // (We multiply the direction vector by a small factor to make the line visible.)
    int endX = playerMiniX + (int)(dirX * 8);
    int endY = playerMiniY + (int)(dirY * 8);
    draw_line(img, playerMiniX, playerMiniY, endX, endY, ft_pixel(0, 0, 255, 255));
}

// --- update_frame: Called every frame.
void update_frame(void *param)
{
    (void)param; // Unused parameter

    // Clear the image by filling it with black.
    for (uint32_t x = 0; x < WIDTH; x++)
    {
        for (uint32_t y = 0; y < HEIGHT; y++)
        {
            mlx_put_pixel(img, x, y, 0x000000FF);
        }
    }

    // --- Raycasting loop: Render the 3D view ---
    for (int x = 0; x < WIDTH; x++)
    {
        // Calculate ray position and direction.
        double cameraX = 2 * x / (double)WIDTH - 1; // x-coordinate in camera space.
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        // Which map square are we in?
        int mapX = (int)posX;
        int mapY = (int)posY;

        // Length of ray from current position to next x or y side.
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

        double sideDistX, sideDistY;
        int stepX, stepY;

        // Calculate step and initial sideDist.
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // Perform DDA.
        int hit = 0; // was there a wall hit?
        int side;    // was the wall vertical or horizontal?
        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall.
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }

        // Calculate distance to the wall.
        double perpWallDist;
        if (side == 0)
            perpWallDist = sideDistX - deltaDistX;
        else
            perpWallDist = sideDistY - deltaDistY;

        // Calculate height of the line to draw.
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill.
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;

        // Choose wall color.
        uint32_t color;
        if (worldMap[mapY][mapX] == 1)
            color = 0xFF0000FF; // Red walls.
        else
            color = 0x00FF00FF; // (Other wall types, if any.)

        // Give y-sides a darker color.
        if (side == 1)
            color = (color >> 1) & 0x7F7F7FFF;

        // Draw the vertical stripe.
        for (int y = drawStart; y <= drawEnd; y++)
        {
            mlx_put_pixel(img, x, y, color);
        }
    }

    // --- Overlay the minimap in the top-left corner ---
    draw_minimap(img);
}

int main(void)
{
    // Initialize MLX42.
    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster with Minimap", true);
    if (!mlx)
    {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    // Create the image for the 3D view.
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
    {
        mlx_close_window(mlx);
        return EXIT_FAILURE;
    }
    mlx_image_to_window(mlx, img, 0, 0);

    // Set our update_frame function as the loop hook.
    mlx_loop_hook(mlx, update_frame, mlx);

    // Start the MLX42 loop.
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}
