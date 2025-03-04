#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <MLX42/MLX42.h>
# include <stdint.h>

typedef struct s_player {
	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double moveSpeed;
	double rotSpeed;
} t_player;

typedef struct s_graphics {
	mlx_t *mlx;
	mlx_image_t *img;
	mlx_texture_t *texNorth;
	mlx_texture_t *texSouth;
	mlx_texture_t *texEast;
	mlx_texture_t *texWest;
} t_graphics;

typedef struct s_config {
	char *texNorthPath;
	char *texSouthPath;
	char *texEastPath;
	char *texWestPath;
	uint32_t floorColor;
	uint32_t ceilingColor;
} t_config;

typedef struct s_map {
	int **world;
	int width;
	int height;
} t_map;

typedef struct s_app {
	t_player player;
	t_graphics gfx;
	t_config config;
	t_map map;
} t_app;

/* New structures for raycasting */
typedef struct s_ray_data {
	double rayDirX;
	double rayDirY;
	int mapX;
	int mapY;
	double deltaDistX;
	double deltaDistY;
	double sideDistX;
	double sideDistY;
	int stepX;
	int stepY;
	int side;
} t_ray_data;

typedef struct s_tex_info {
	int texX;
	int texWidth;
	int texHeight;
	mlx_texture_t *texture;
} t_tex_info;

typedef struct s_column_draw {
	int x;
	int drawStart;
	int drawEnd;
	int lineHeight;
	int side;
	double rayDirX;
	double rayDirY;
	double wallX;
} t_column_draw;

/* Linked list for storing map lines (during parsing) */
typedef struct s_mapLine {
	char				*line;
	struct s_mapLine	*next;
}	t_mapLine;

/* Chunk structure for file reading */
typedef struct s_chunk {
	char			*data;
	int				size;
	struct s_chunk	*next;
}	t_chunk;

#endif