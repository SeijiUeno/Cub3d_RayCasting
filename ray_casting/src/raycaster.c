// raycaster.c
#include <MLX42/MLX42.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Screen dimensions
#define WIDTH 512
#define HEIGHT 512

// Map dimensions
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// The world map: 1 represents a wall, 0 is empty space.
// (This is the classic map used in many raycasting tutorials.)
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

// Player starting position and direction
double posX = 22, posY = 12;       // Player's position in map units
double dirX = -1, dirY = 0;         // Initial direction vector (normalized)
double planeX = 0, planeY = 0.66;    // Camera plane (perpendicular to dir); FOV = 2*atan(0.66)=66°

// Global MLX42 objects
mlx_t *mlx;
static mlx_image_t *img;

// The update_frame function is called every frame.
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

    // Raycasting loop: for every vertical stripe (column) of the screen.
    for (int x = 0; x < WIDTH; x++)
    {
        // --- Step 1: Calculate ray position and direction ---
        // cameraX maps x-coordinate to range [-1, 1]:
        double cameraX = 2 * x / (double)WIDTH - 1;
        // The ray direction is the player's direction plus an offset from the camera plane.
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        // --- Step 2: Which map square are we in? ---
        int mapX = (int)posX;
        int mapY = (int)posY;

        // --- Step 3: Calculate the distance to the next x and y side ---
        // Delta distance: distance from one x-side to the next x-side (or y-side).
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

        double sideDistX, sideDistY;
        int stepX, stepY;

        // --- Step 4: Calculate step and initial sideDist ---
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

        // --- Step 5: Perform DDA (Digital Differential Analyzer) ---
        int hit = 0;  // Was a wall hit?
        int side;     // Was a NS or EW wall hit? (0 for x-side, 1 for y-side)
        while (hit == 0)
        {
            // Jump to next map square, either in x or y direction.
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
            // Check if ray has hit a wall (non-zero value in worldMap).
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }

        // --- Step 6: Calculate distance to the wall ---
        double perpWallDist;
        if (side == 0)
            perpWallDist = sideDistX - deltaDistX;
        else
            perpWallDist = sideDistY - deltaDistY;

        // --- Step 7: Calculate height of line to draw on screen ---
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // Calculate lowest and highest pixel to fill in the current stripe.
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;

        // --- Step 8: Choose wall color ---
        // For now, color depends on the wall type.
        uint32_t color;
        if (worldMap[mapY][mapX] == 1)
            color = 0xFF0000FF; // Red walls
        else
            color = 0x00FF00FF; // Green (if other types exist)

        // Give y-sides a darker shade to add simple shading.
        if (side == 1)
        {
            // A simple way to darken: shift right one bit.
            color = (color >> 1) & 0x7F7F7FFF;
        }

        for (int y = drawStart; y <= drawEnd; y++)
        {
            mlx_put_pixel(img, x, y, color);
        }
    }
}

int main(void)
{
    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster", true);
    if (!mlx)
    {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
    {
        mlx_close_window(mlx);
        return EXIT_FAILURE;
    }

    mlx_image_to_window(mlx, img, 0, 0);

    mlx_loop_hook(mlx, update_frame, mlx);

    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}