#include <MLX42/MLX42.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

//===================================================================
// Screen Dimensions & Clear Color
//===================================================================
#define WIDTH 1280
#define HEIGHT 720
#define CLEAR_COLOR 0x000000FF  // Black

//===================================================================
// Minimap Settings
//===================================================================
#define MINIMAP_SCALE 6

//===================================================================
// Global Map Variables (dynamically allocated)
//===================================================================
int **worldMap = NULL;
int g_mapWidth = 0, g_mapHeight = 0;

//===================================================================
// Global Player State
//===================================================================
double posX, posY;      // Player position
double dirX, dirY;      // Direction vector
double planeX, planeY;  // Camera plane (for FOV)

double moveSpeed = 0.05;
double rotSpeed  = 0.03;

//===================================================================
// Global MLX42 Objects
//===================================================================
mlx_t *mlx;
static mlx_image_t *img;

//===================================================================
// Texture Global Variables & Paths
//===================================================================
mlx_texture_t *texNorth, *texSouth, *texEast, *texWest;
char *texNorthPath = NULL;
char *texSouthPath = NULL;
char *texEastPath  = NULL;
char *texWestPath  = NULL;

//===================================================================
// Floor and Ceiling Colors (parsed from file)
//===================================================================
uint32_t floorColor = 0x006400FF;   // Default dark green
uint32_t ceilingColor = 0x87CEEBFF; // Default sky blue

//===================================================================
// Utility: Pack RGBA values into a 32-bit integer.
//===================================================================
int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

//===================================================================
// Linked List for Map Lines (to avoid using realloc)
//===================================================================
typedef struct MapLine {
    char *line;
    struct MapLine *next;
} MapLine;

//===================================================================
// Cleanup: Free allocated resources before exiting.
//===================================================================
void cleanup(void) {
    // Free worldMap
    if (worldMap) {
        for (int i = 0; i < g_mapHeight; i++) {
            free(worldMap[i]);
        }
        free(worldMap);
    }
    // Free texture path strings.
    free(texNorthPath);
    free(texSouthPath);
    free(texEastPath);
    free(texWestPath);
}

//===================================================================
// parse_cub_file: Reads a .cub file and sets configuration and map.
// Avoids realloc by using a linked list to store map lines.
//===================================================================
void parse_cub_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening cub file");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int configCount = 0;
    int mapStarted = 0;
    MapLine *head = NULL, *tail = NULL;
    int mapLineCount = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        // Remove newline characters.
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len == 0)
            continue; // Skip blank lines

        if (!mapStarted) {
            if (strncmp(buffer, "NO ", 3) == 0) {
                texNorthPath = strdup(buffer + 3);
                if (!texNorthPath) { perror("strdup error"); exit(EXIT_FAILURE); }
                configCount++;
                continue;
            } else if (strncmp(buffer, "SO ", 3) == 0) {
                texSouthPath = strdup(buffer + 3);
                if (!texSouthPath) { perror("strdup error"); exit(EXIT_FAILURE); }
                configCount++;
                continue;
            } else if (strncmp(buffer, "WE ", 3) == 0) {
                texWestPath = strdup(buffer + 3);
                if (!texWestPath) { perror("strdup error"); exit(EXIT_FAILURE); }
                configCount++;
                continue;
            } else if (strncmp(buffer, "EA ", 3) == 0) {
                texEastPath = strdup(buffer + 3);
                if (!texEastPath) { perror("strdup error"); exit(EXIT_FAILURE); }
                configCount++;
                continue;
            } else if (buffer[0] == 'F' && buffer[1] == ' ') {
                int r, g, b;
                if (sscanf(buffer + 2, "%d,%d,%d", &r, &g, &b) == 3) {
                    floorColor = ft_pixel(r, g, b, 255);
                    configCount++;
                    continue;
                } else {
                    fprintf(stderr, "Error parsing floor color\n");
                    exit(EXIT_FAILURE);
                }
            } else if (buffer[0] == 'C' && buffer[1] == ' ') {
                int r, g, b;
                if (sscanf(buffer + 2, "%d,%d,%d", &r, &g, &b) == 3) {
                    ceilingColor = ft_pixel(r, g, b, 255);
                    configCount++;
                    continue;
                } else {
                    fprintf(stderr, "Error parsing ceiling color\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Any other line marks the beginning of the map.
                mapStarted = 1;
            }
        }
        if (mapStarted) {
            // Allocate a new node for this map line.
            MapLine *node = malloc(sizeof(MapLine));
            if (!node) {
                perror("malloc error");
                exit(EXIT_FAILURE);
            }
            node->line = strdup(buffer);
            if (!node->line) {
                perror("strdup error");
                exit(EXIT_FAILURE);
            }
            node->next = NULL;
            if (!head)
                head = node;
            else
                tail->next = node;
            tail = node;
            mapLineCount++;
        }
    }
    fclose(fp);

    if (configCount < 6) {  // Expecting 4 textures + floor + ceiling
        fprintf(stderr, "Incomplete configuration in cub file.\n");
        exit(EXIT_FAILURE);
    }
    if (mapLineCount == 0) {
        fprintf(stderr, "No map data found in cub file.\n");
        exit(EXIT_FAILURE);
    }

    // Determine map dimensions.
    g_mapHeight = mapLineCount;
    g_mapWidth = 0;
    for (MapLine *node = head; node; node = node->next) {
        int lineLen = strlen(node->line);
        if (lineLen > g_mapWidth)
            g_mapWidth = lineLen;
    }

    // Allocate the worldMap.
    worldMap = malloc(sizeof(int*) * g_mapHeight);
    if (!worldMap) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < g_mapHeight; i++) {
        worldMap[i] = malloc(sizeof(int) * g_mapWidth);
        if (!worldMap[i]) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < g_mapWidth; j++)
            worldMap[i][j] = 0;
    }

    int currentLine = 0;
    int playerFound = 0;
    // Transfer each linked list node into worldMap.
    for (MapLine *node = head; node; node = node->next, currentLine++) {
        for (int j = 0; j < g_mapWidth; j++) {
            char cell = ' ';
            if (j < (int)strlen(node->line))
                cell = node->line[j];
            if (cell >= '0' && cell <= '9') {
                worldMap[currentLine][j] = cell - '0';
            } else if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W') {
                posX = j + 0.5;
                posY = currentLine + 0.5;
                if (cell == 'N') {
                    dirX = 0;   dirY = -1;
                    planeX = 0.66; planeY = 0;
                } else if (cell == 'S') {
                    dirX = 0;   dirY = 1;
                    planeX = -0.66; planeY = 0;
                } else if (cell == 'E') {
                    dirX = 1;   dirY = 0;
                    planeX = 0;  planeY = 0.66;
                } else if (cell == 'W') {
                    dirX = -1;  dirY = 0;
                    planeX = 0;  planeY = -0.66;
                }
                worldMap[currentLine][j] = 0;
                playerFound++;
            } else if (cell == ' ') {
                worldMap[currentLine][j] = 0;
            } else {
                fprintf(stderr, "Invalid map character '%c' at (%d, %d)\n", cell, currentLine, j);
                exit(EXIT_FAILURE);
            }
        }
        free(node->line);  // Free the line string.
    }
    // Free the linked list nodes.
    while (head) {
        MapLine *temp = head;
        head = head->next;
        free(temp);
    }

    if (playerFound != 1) {
        fprintf(stderr, "Error: Map must have exactly one spawn; found %d\n", playerFound);
        exit(EXIT_FAILURE);
    }
}

//===================================================================
// Utility Functions
//===================================================================

// Clears the image by setting every pixel to the given color.
void clear_image(mlx_image_t *img, uint32_t color) {
    size_t total = (size_t)img->width * img->height;
    uint32_t *pixels = (uint32_t *)img->pixels;
    for (size_t i = 0; i < total; i++)
        pixels[i] = color;
}

// Draws a line between (x0, y0) and (x1, y1) using Bresenham's algorithm.
void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = abs(x1 - x0), dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        if ((uint32_t)x0 < img->width && (uint32_t)y0 < img->height)
            mlx_put_pixel(img, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

//===================================================================
// Rotation: Rotates the player's direction and camera plane vectors.
//===================================================================
static void rotate(double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    double oldDirX = dirX;
    dirX = dirX * cosA - dirY * sinA;
    dirY = oldDirX * sinA + dirY * cosA;
    double oldPlaneX = planeX;
    planeX = planeX * cosA - planeY * sinA;
    planeY = oldPlaneX * sinA + planeY * cosA;
    double dirLen = sqrt(dirX * dirX + dirY * dirY);
    if (dirLen != 0) {
        dirX /= dirLen;
        dirY /= dirLen;
    }
    double planeLen = sqrt(planeX * planeX + planeY * planeY);
    if (planeLen != 0) {
        planeX = (planeX / planeLen) * 0.66;
        planeY = (planeY / planeLen) * 0.66;
    }
}

//===================================================================
// Input Handling: Process keyboard input for movement and rotation.
//===================================================================
void process_input(void) {
    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    // Move forward.
    if (mlx_is_key_down(mlx, MLX_KEY_W)) {
        double newX = posX + dirX * moveSpeed;
        double newY = posY + dirY * moveSpeed;
        if (newX >= 0 && newX < g_mapWidth && worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (newY >= 0 && newY < g_mapHeight && worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Move backward.
    if (mlx_is_key_down(mlx, MLX_KEY_S)) {
        double newX = posX - dirX * moveSpeed;
        double newY = posY - dirY * moveSpeed;
        if (newX >= 0 && newX < g_mapWidth && worldMap[(int)posY][(int)newX] == 0)
            posX = newX;
        if (newY >= 0 && newY < g_mapHeight && worldMap[(int)newY][(int)posX] == 0)
            posY = newY;
    }
    // Rotate left.
    if (mlx_is_key_down(mlx, MLX_KEY_A))
        rotate(rotSpeed);
    // Rotate right.
    if (mlx_is_key_down(mlx, MLX_KEY_D))
        rotate(-rotSpeed);
}

//===================================================================
// 3D Raycasting Rendering
//===================================================================
void render_3d_view(mlx_image_t *img) {
    for (int x = 0; x < WIDTH; x++) {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;
        int mapX = (int)posX, mapY = (int)posY;
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double sideDistX, sideDistY;
        int stepX, stepY;
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
        int hit = 0, side;
        double perpWallDist = 0.0;
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
            if (mapX < 0 || mapX >= g_mapWidth || mapY < 0 || mapY >= g_mapHeight) {
                hit = 1;
                perpWallDist = HEIGHT;
                break;
            }
            if (worldMap[mapY][mapX] > 0)
                hit = 1;
        }
        if (mapX >= 0 && mapX < g_mapWidth && mapY >= 0 && mapY < g_mapHeight)
            perpWallDist = (side == 0) ? sideDistX - deltaDistX : sideDistY - deltaDistY;
        if (perpWallDist < 0.2)
            perpWallDist = 0.2;
        int lineHeight = (int)(HEIGHT / perpWallDist);
        if (lineHeight <= 0)
            lineHeight = 1;
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;
        double wallX = (side == 0) ? posY + perpWallDist * rayDirY
                                   : posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);
        mlx_texture_t *texture = (side == 0)
            ? (rayDirX < 0 ? texEast : texWest)
            : (rayDirY < 0 ? texSouth : texNorth);
        int texWidth = texture->width;
        int texHeight = texture->height;
        int texX = (int)(wallX * texWidth);
        if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
            texX = texWidth - texX - 1;
        for (int y = 0; y < HEIGHT; y++) {
            if (y < drawStart)
                mlx_put_pixel(img, x, y, ceilingColor);
            else if (y <= drawEnd) {
                int d = y * 256 - HEIGHT * 128 + lineHeight * 128;
                int texY = ((d * texHeight) / lineHeight) / 256;
                if (texY < 0) texY = 0;
                if (texY >= texHeight) texY = texHeight - 1;
                uint32_t wallColor = ((uint32_t *)texture->pixels)[texY * texWidth + texX];
                mlx_put_pixel(img, x, y, wallColor);
            } else {
                mlx_put_pixel(img, x, y, floorColor);
            }
        }
    }
}

//===================================================================
// Minimap Rendering
//===================================================================
void render_minimap(mlx_image_t *img) {
    for (int my = 0; my < g_mapHeight; my++) {
        for (int mx = 0; mx < g_mapWidth; mx++) {
            uint32_t color = (worldMap[my][mx] > 0) ?
                ft_pixel(0, 0, 0, 255) : ft_pixel(255, 255, 255, 255);
            for (int y = 0; y < MINIMAP_SCALE; y++) {
                for (int x = 0; x < MINIMAP_SCALE; x++) {
                    int px = mx * MINIMAP_SCALE + x;
                    int py = my * MINIMAP_SCALE + y;
                    if ((uint32_t)px < img->width && (uint32_t)py < img->height)
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
            if ((uint32_t)px < img->width && (uint32_t)py < img->height)
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
void update_frame(void *param) {
    (void)param;
    process_input();
    clear_image(img, CLEAR_COLOR);
    render_3d_view(img);
    render_minimap(img);
}

//===================================================================
// Main Function
//===================================================================
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.cub\n", argv[0]);
        return EXIT_FAILURE;
    }
    atexit(cleanup);
    parse_cub_file(argv[1]);

    mlx = mlx_init(WIDTH, HEIGHT, "Raycaster", true);
    if (!mlx) {
        fprintf(stderr, "MLX init error\n");
        return EXIT_FAILURE;
    }

    texNorth = mlx_load_png("east.png");
    texSouth = mlx_load_png("north.png");
    texEast  = mlx_load_png("south.png");
    texWest  = mlx_load_png("west.png");
    if (!texNorth || !texSouth || !texEast || !texWest) {
        fprintf(stderr, "Error: One or more textures failed to load.\n");
        return EXIT_FAILURE;
    }

    img = mlx_new_image(mlx, WIDTH, HEIGHT);
    if (!img) {
        mlx_close_window(mlx);
        return EXIT_FAILURE;
    }
    mlx_image_to_window(mlx, img, 0, 0);

    mlx_loop_hook(mlx, update_frame, mlx);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return EXIT_SUCCESS;
}
