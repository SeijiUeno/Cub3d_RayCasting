#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <MLX42/MLX42.h>
# include <stdint.h>

/* Player structure */
typedef struct s_player {
	double	posX;
	double	posY;
	double	dirX;
	double	dirY;
	double	planeX;
	double	planeY;
	double	moveSpeed;
	double	rotSpeed;
}	t_player;

/* Graphics structure */
typedef struct s_graphics {
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_texture_t	*texNorth;
	mlx_texture_t	*texSouth;
	mlx_texture_t	*texEast;
	mlx_texture_t	*texWest;
}	t_graphics;

/* Configuration structure */
typedef struct s_config {
	char		*texNorthPath;
	char		*texSouthPath;
	char		*texEastPath;
	char		*texWestPath;
	uint32_t	floorColor;
	uint32_t	ceilingColor;
}	t_config;

/* Map structure */
typedef struct s_map {
	int		**world;
	int		width;
	int		height;
}	t_map;

/* Global application state */
typedef struct s_app {
	t_player	player;
	t_graphics	gfx;
	t_config	config;
	t_map		map;
}	t_app;

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
