#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

//===================================================================
// Constants and Global Variables
//===================================================================

// Screen dimensions.
#define WIDTH 512
#define HEIGHT 512

// Map dimensions.
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// Minimap scale: each map cell is drawn as a MINIMAP_SCALE x MINIMAP_SCALE block.
#define MINIMAP_SCALE 4

// Colors for ceiling and floor.
#define CEILING_COLOR 0x87CEEBFF  // Sky Blue.
#define FLOOR_COLOR   0x006400FF  // Dark Green.

// Global world map (populated by parse_map).
// Allowed characters: '0' for empty, '1' for wall, 'N','S','E','W' for player spawn.
int worldMap[MAP_HEIGHT][MAP_WIDTH];

// Player state (set via map parsing).
double posX, posY;      // Player position (in map units).
double dirX, dirY;      // Direction vector.
double planeX, planeY;  // Camera plane (defines FOV).

// Movement parameters.
double moveSpeed = 0.05;
double rotSpeed  = 0.03;

// Global MLX42 objects.
mlx_t *mlx;
static mlx_image_t *img;

//-------------------------------------------------------------------
// Texture Global Variables (wall textures).
mlx_texture_t *texNorth;
mlx_texture_t *texSouth;
mlx_texture_t *texEast;
mlx_texture_t *texWest;

//===================================================================
// Map Parsing
//===================================================================

// Define the map as an array of 24 strings (each 24 characters long).
const char *mapData[MAP_HEIGHT] = {
    "111111111111111111111111",
    "1N0000000000000000000001",
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

void parse_map(void)
{
    int playerFound = 0;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            char ch = mapData[i][j];
            if (ch >= '0' && ch <= '9') {
                worldMap[i][j] = ch - '0';
            } else if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W') {
                // Set player position to the center of this cell.
                posX = j + 0.5;
                posY = i + 0.5;
                // Set player orientation based on spawn.
                if (ch == 'N') {
                    dirX = 0;   dirY = -1;
                    planeX = 0.66; planeY = 0;
                } else if (ch == 'S') {
                    dirX = 0;   dirY = 1;
                    planeX = -0.66; planeY = 0;
                } else if (ch == 'E') {
                    dirX = 1;   dirY = 0;
                    planeX = 0; planeY = 0.66;
                } else if (ch == 'W') {
                    dirX = -1;  dirY = 0;
                    planeX = 0; planeY = -0.66;
                }
                worldMap[i][j] = 0; // Mark spawn cell as floor.
                playerFound++;
            } else {
                fprintf(stderr, "Invalid map character '%c' at (%d,%d)\n", ch, i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (playerFound != 1) {
        fprintf(stderr, "Error: Map must have exactly one spawn; found %d\n", playerFound);
        exit(EXIT_FAILURE);
    }
}

//===================================================================
// Utility Functions
//===================================================================

// Packs an RGBA color into a 32-bit integer.
static inline uint32_t ft_pixel(int r, int g, int b, int a)
{
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

// Draws a line using Bresenham's algorithm.
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
            mlx_put_pixel(img, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

//===================================================================
// Input Handling with Rotation Normalization
//===================================================================

void process_input(void)
{
    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    // Move forward (W).
    if (mlx_is_key_down(mlx, MLX_KEY_W)) {
        double newX = posX + dirX * moveSpeed;
        double newY = posY + dirY * moveSpeed;
        if (worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Move backward (S).
    if (mlx_is_key_down(mlx, MLX_KEY_S)) {
        double newX = posX - dirX * moveSpeed;
        double newY = posY - dirY * moveSpeed;
        if (worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Rotate Left (A).
    if (mlx_is_key_down(mlx, MLX_KEY_A)) {
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        // Normalize direction vector.
        double len = sqrt(dirX * dirX + dirY * dirY);
        if (len != 0) { dirX /= len; dirY /= len; }
        // Normalize plane vector and rescale to maintain FOV.
        len = sqrt(planeX * planeX + planeY * planeY);
        if (len != 0) { planeX /= len; planeY /= len; planeX *= 0.66; planeY *= 0.66; }
    }
    // Rotate Right (D).
    if (mlx_is_key_down(mlx, MLX_KEY_D)) {
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        // Normalize direction vector.
        double len = sqrt(dirX * dirX + dirY * dirY);
        if (len != 0) { dirX /= len; dirY /= len; }
        // Normalize plane vector and rescale.
        len = sqrt(planeX * planeX + planeY * planeY);
        if (len != 0) { planeX /= len; planeY /= len; planeX *= 0.66; planeY *= 0.66; }
    }
}

//===================================================================
// 3D Raycasting Rendering (Without Fog)
//===================================================================

void render_3d_view(mlx_image_t *img)
{
    for (int x = 0; x < WIDTH; x++) {
        // 1. Calculate ray position and direction.
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        // 2. Determine current map square.
        int mapX = (int)posX;
        int mapY = (int)posY;

        // 3. Calculate delta distances.
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double sideDistX, sideDistY;
        int stepX, stepY;

        // 4. Calculate step and initial side distances.
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // 5. Perform DDA with boundary check.
        double perpWallDist = 0.0;
        int hit = 0, side;
        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Boundary check.
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                hit = 1;
                perpWallDist = HEIGHT;  // Fallback distance.
                break;
            }
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }
        if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT)
            perpWallDist = (side == 0) ? sideDistX - deltaDistX : sideDistY - deltaDistY;

        // 6. Calculate wall slice height.
        int lineHeight = (int)(HEIGHT / perpWallDist);
        if (lineHeight <= 0)
            lineHeight = 1;

        // 7. Calculate draw start and end positions.
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // 8. Calculate wall hit position (wallX).
        double wallX;
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        // 9. Choose texture based on side and ray direction.
        mlx_texture_t *texture;
        if (side == 0)
            texture = (rayDirX < 0) ? texEast : texWest;
        else
            texture = (rayDirY < 0) ? texSouth : texNorth;

        int texWidth = texture->width;
        int texHeight = texture->height;
        int texX = (int)(wallX * texWidth);
        if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        // 10. Draw the vertical stripe.
        for (int y = 0; y < HEIGHT; y++) {
            if (y < drawStart) {
                // Ceiling.
                mlx_put_pixel(img, x, y, CEILING_COLOR);
            } else if (y <= drawEnd) {
                // Calculate texture y-coordinate.
                int d = y * 256 - HEIGHT * 128 + lineHeight * 128;
                int texY = ((d * texHeight) / lineHeight) / 256;
                if (texY < 0) texY = 0;
                if (texY >= texHeight) texY = texHeight - 1;
                uint32_t wallColor = ((uint32_t *)texture->pixels)[texY * texWidth + texX];
                mlx_put_pixel(img, x, y, wallColor);
            } else {
                // Floor.
                mlx_put_pixel(img, x, y, FLOOR_COLOR);
            }
        }
    }
}

//===================================================================
// Minimap Rendering (unchanged)
//===================================================================

void render_minimap(mlx_image_t *img)
{
    for (int my = 0; my < MAP_HEIGHT; my++) {
        for (int mx = 0; mx < MAP_WIDTH; mx++) {
            uint32_t color = (worldMap[my][mx] > 0) ?
                ft_pixel(0, 0, 0, 255) : ft_pixel(255, 255, 255, 255);
            for (int y = 0; y < MINIMAP_SCALE; y++) {
                for (int x = 0; x < MINIMAP_SCALE; x++) {
                    int px = mx * MINIMAP_SCALE + x;
                    int py = my * MINIMAP_SCALE + y;
                    if (px < (int)img->width && py < (int)img->height)
                        mlx_put_pixel(img, px, py, color);
                }
            }
        }
    }
    int playerMiniX = (int)(posX * MINIMAP_SCALE);
    int playerMiniY = (int)(posY * MINIMAP_SCALE);
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int px = playerMiniX + dx;
            int py = playerMiniY + dy;
            if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
                mlx_put_pixel(img, px, py, ft_pixel(255, 0, 0, 255));
        }
    }
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
    // 1. Parse the map to set up worldMap and player spawn.
    parse_map();

    // 2. Initialize MLX42.
    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster without Fog", true);
    if (!mlx) {
        puts("MLX init error");
        return EXIT_FAILURE;
    }

    // 3. Load wall textures.
    texNorth = mlx_load_png("src/north.png");
    texSouth = mlx_load_png("src/south.png");
    texEast  = mlx_load_png("src/east.png");
    texWest  = mlx_load_png("src/west.png");
    if (!texNorth || !texSouth || !texEast || !texWest) {
        fprintf(stderr, "Error: One or more wall textures failed to load.\n");
        return EXIT_FAILURE;
    }

    // 4. Create the image for rendering.
    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img) {
        mlx_close_window(mlx);
        return EXIT_FAILURE;
    }
    mlx_image_to_window(mlx, img, 0, 0);

    // 5. Set the update frame hook and start the main loop.
    mlx_loop_hook(mlx, update_frame, mlx);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}
