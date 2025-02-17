#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

//===================================================================
// Constants and Global Variables
//===================================================================

// Screen dimensions
#define WIDTH 512
#define HEIGHT 512

// Map dimensions for raycasting (classic 24x24 map)
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Minimap scale: each map cell becomes a MINIMAP_SCALE x MINIMAP_SCALE block.
#define MINIMAP_SCALE 4

// Floor and ceiling colors (RGBA)
#define CEILING_COLOR 0x87CEEBFF  // Sky Blue: (135,206,235,255)
#define FLOOR_COLOR   0x006400FF  // Dark Green: (0,100,0,255)

//-------------------------------------------------------------------
// World Map: 1 = wall, 0 = empty space.
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

//-------------------------------------------------------------------
// Player state: position, direction, and camera plane.
double posX = 10, posY = 20;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;

// Movement parameters.
double moveSpeed = 0.05;  // Movement speed
double rotSpeed  = 0.03;  // Rotation speed

// Global MLX42 objects.
mlx_t *mlx;
static mlx_image_t *img;

//===================================================================
// Utility Functions
//===================================================================

// Pack an RGBA color into a 32-bit integer.
static inline uint32_t ft_pixel(int r, int g, int b, int a)
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

// Draw a line using Bresenham's algorithm.
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

//===================================================================
// Input Handling
//===================================================================

void process_input(void)
{
    // Quit if ESC is pressed.
    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    // Move forward (W) with collision checking.
    if (mlx_is_key_down(mlx, MLX_KEY_W))
    {
        double newX = posX + dirX * moveSpeed;
        double newY = posY + dirY * moveSpeed;
        if (worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Move backward (S).
    if (mlx_is_key_down(mlx, MLX_KEY_S))
    {
        double newX = posX - dirX * moveSpeed;
        double newY = posY - dirY * moveSpeed;
        if (worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Rotate left (A).
    if (mlx_is_key_down(mlx, MLX_KEY_A))
    {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
    // Rotate right (D).
    if (mlx_is_key_down(mlx, MLX_KEY_D))
    {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
}

//===================================================================
// 3D Raycasting Rendering
//===================================================================

void render_3d_view(mlx_image_t *img)
{
    // For each vertical stripe on the screen.
    for (int x = 0; x < WIDTH; x++)
    {
        // 1. Calculate ray position and direction.
        double cameraX = 2 * x / (double)WIDTH - 1;  // x-coordinate in camera space.
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        // 2. Which map square are we in?
        int mapX = (int)posX;
        int mapY = (int)posY;

        // 3. Calculate delta distances.
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

        double sideDistX, sideDistY;
        int stepX, stepY;

        // 4. Calculate step and initial sideDist.
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

        // 5. Perform DDA.
        int hit = 0;  // Has a wall been hit?
        int side;     // 0 for an X-side, 1 for a Y-side.
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
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }

        // 6. Calculate distance to the wall (perpendicular distance).
        double perpWallDist;
        if (side == 0)
            perpWallDist = sideDistX - deltaDistX;
        else
            perpWallDist = sideDistY - deltaDistY;

        // 7. Calculate height of the line to draw.
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // 8. Calculate lowest and highest pixel to fill for the current stripe.
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;

        // 9. Choose wall color.
        uint32_t wallColor;
        if (worldMap[mapY][mapX] == 1)
            wallColor = 0xFF0000FF;  // Red walls.
        else
            wallColor = 0x00FF00FF;  // Other types (if any).
        if (side == 1)
            wallColor = (wallColor >> 1) & 0x7F7F7FFF; // Darken for y-sides.

        // 10. Draw the column:
        //    - Ceiling: from y = 0 to drawStart-1.
        //    - Wall: from y = drawStart to drawEnd.
        //    - Floor: from y = drawEnd+1 to HEIGHT-1.
        for (int y = 0; y < drawStart; y++)
            mlx_put_pixel(img, x, y, CEILING_COLOR);
        for (int y = drawStart; y <= drawEnd; y++)
            mlx_put_pixel(img, x, y, wallColor);
        for (int y = drawEnd + 1; y < HEIGHT; y++)
            mlx_put_pixel(img, x, y, FLOOR_COLOR);
    }
}

//===================================================================
// Minimap Rendering
//===================================================================

void render_minimap(mlx_image_t *img)
{
    // Draw each map cell on the minimap.
    for (int my = 0; my < MAP_HEIGHT; my++)
    {
        for (int mx = 0; mx < MAP_WIDTH; mx++)
        {
            uint32_t color = (worldMap[my][mx] > 0) ?
                ft_pixel(0, 0, 0, 255) : ft_pixel(255, 255, 255, 255);
            // Fill a rectangle for this cell.
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
    int endX = playerMiniX + (int)(dirX * 8);
    int endY = playerMiniY + (int)(dirY * 8);
    draw_line(img, playerMiniX, playerMiniY, endX, endY, ft_pixel(0, 0, 255, 255));
}

//===================================================================
// Main Frame Update
//===================================================================

void update_frame(void *param)
{
    (void)param; // Unused parameter

    // Process player input.
    process_input();

    // Render the 3D raycasting view.
    render_3d_view(img);

    // Overlay the minimap in the top-left corner.
    render_minimap(img);
}

//===================================================================
// Main Function
//===================================================================

int main(void)
{
    // Initialize MLX42.
    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster with Minimap, Movement & Floor/Ceiling", true);
    if (!mlx)
    {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    // Create the image that will hold our rendered frame.
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img)
    {
        mlx_close_window(mlx);
        return EXIT_FAILURE;
    }
    mlx_image_to_window(mlx, img, 0, 0);

    // Set update_frame as the loop hook.
    mlx_loop_hook(mlx, update_frame, mlx);

    // Start the MLX42 loop.
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}
