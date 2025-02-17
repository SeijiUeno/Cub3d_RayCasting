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

// Map dimensions (24x24)
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Minimap scale: each map cell is drawn as a MINIMAP_SCALE x MINIMAP_SCALE block.
#define MINIMAP_SCALE 4

// Floor and ceiling colors (RGBA)
#define CEILING_COLOR 0x87CEEBFF  // Sky Blue (135,206,235,255)
#define FLOOR_COLOR   0x006400FF  // Dark Green (0,100,0,255)

//-------------------------------------------------------------------
// Global world map (filled via parse_map)
// Allowed characters:
//   '0' : empty space,
//   '1' : wall,
//   'N','S','E','W' : player spawn (and orientation)
int worldMap[MAP_HEIGHT][MAP_WIDTH];

// Player state: position, direction, and camera plane.
double posX, posY;      // In map units (will be set by the map parser)
double dirX, dirY;      // Player's direction vector
double planeX, planeY;  // Camera plane (perpendicular to direction)

// Movement parameters.
double moveSpeed = 0.05;  // Movement speed
double rotSpeed  = 0.03;  // Rotation speed

// Global MLX42 objects.
mlx_t *mlx;
static mlx_image_t *img;

//===================================================================
// Map Parsing
//===================================================================

// Define your map as an array of strings (each string must have MAP_WIDTH characters).
const char *mapData[MAP_HEIGHT] = {
    "111111111111111111111111",
    "1E0000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "100000000000000000000001",
    "111111111111111111111111"
};

// Parses the map and fills the global worldMap array.
// Also, sets the player's starting position and orientation based on the spawn character.
// If a spawn character ('N','S','E','W') is encountered, that cell is set to '0' (floor).
void parse_map(void)
{
    int playerFound = 0;

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            char ch = mapData[i][j];

            // If the character is a digit ('0'-'9'):
            if (ch >= '0' && ch <= '9')
            {
                worldMap[i][j] = ch - '0';
            }
            // If the character indicates a spawn position.
            else if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
            {
                // Set the player's position to the center of this cell.
                posX = j + 0.5;
                posY = i + 0.5;

                // Set the player's orientation based on the spawn character.
                if (ch == 'N')
                {
                    // Facing north (upward on the screen; negative Y direction)
                    dirX = 0;
                    dirY = -1;
                    // The camera plane is perpendicular to the direction.
                    planeX = 0.66;
                    planeY = 0;
                }
                else if (ch == 'S')
                {
                    // Facing south (downward on the screen)
                    dirX = 0;
                    dirY = 1;
                    planeX = -0.66;
                    planeY = 0;
                }
                else if (ch == 'E')
                {
                    // Facing east (to the right)
                    dirX = 1;
                    dirY = 0;
                    planeX = 0;
                    planeY = 0.66;
                }
                else if (ch == 'W')
                {
                    // Facing west (to the left)
                    dirX = -1;
                    dirY = 0;
                    planeX = 0;
                    planeY = -0.66;
                }
                // Replace spawn cell with floor (0) in the map.
                worldMap[i][j] = 0;
                playerFound++;
            }
            else
            {
                fprintf(stderr, "Error: Invalid map character '%c' at (%d, %d)\n", ch, i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (playerFound != 1)
    {
        fprintf(stderr, "Error: Map must contain exactly one player spawn. Found %d\n", playerFound);
        exit(EXIT_FAILURE);
    }
}

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
        double cameraX = 2 * x / (double)WIDTH - 1;
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
        int hit = 0;
        int side;
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

        // 6. Calculate perpendicular wall distance.
        double perpWallDist = (side == 0) ?
            sideDistX - deltaDistX : sideDistY - deltaDistY;

        // 7. Calculate height of line to draw.
        int lineHeight = (int)(HEIGHT / perpWallDist);

        // 8. Calculate lowest and highest pixel to fill.
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // 9. Choose wall color.
        uint32_t wallColor = (worldMap[mapY][mapX] == 1) ?
            0xFF0000FF : 0x00FF00FF;
        if (side == 1)
            wallColor = (wallColor >> 1) & 0x7F7F7FFF;

        // 10. Draw the vertical stripe:
        //    Ceiling
        for (int y = 0; y < drawStart; y++)
            mlx_put_pixel(img, x, y, CEILING_COLOR);
        //    Wall
        for (int y = drawStart; y <= drawEnd; y++)
            mlx_put_pixel(img, x, y, wallColor);
        //    Floor
        for (int y = drawEnd + 1; y < HEIGHT; y++)
            mlx_put_pixel(img, x, y, FLOOR_COLOR);
    }
}

//===================================================================
// Minimap Rendering
//===================================================================

void render_minimap(mlx_image_t *img)
{
    for (int my = 0; my < MAP_HEIGHT; my++)
    {
        for (int mx = 0; mx < MAP_WIDTH; mx++)
        {
            uint32_t color = (worldMap[my][mx] > 0) ?
                ft_pixel(0, 0, 0, 255) : ft_pixel(255, 255, 255, 255);
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

    // Draw the player as a small red square.
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
    (void)param;

    process_input();
    render_3d_view(img);
    render_minimap(img);
}

//===================================================================
// Main Function
//===================================================================

int main(void)
{
    // First, parse the map to fill worldMap and set player spawn.
    parse_map();

    // Initialize MLX42.
    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster with Map Parser", true);
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
