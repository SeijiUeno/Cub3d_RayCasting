/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:40:24 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 17:13:18 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "get_next_line.h"
# include <MLX42/MLX42.h>

# define WIDTH 920
# define HEIGHT 720
# define CLEAR_COLOR 0x000000FF
# define MINIMAP_SCALE 6
# define ERROR "\033[31mError!\033[0m"
# define ROW 100
# define COL 80
# define RAY_INFINITY 1e30
# define RGB 3

enum e_elements
{
	NO = 1,
	SO = 2,
	WE = 4,
	EA = 8,
	F = 16,
	C = 32,
	E_ALL = 63
};

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plan_x;
	double	plan_y;
	double	move_sped;
	double	rot_sped;
}	t_player;

typedef struct s_graphics
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_texture_t	*tex_no;
	mlx_texture_t	*tex_so;
	mlx_texture_t	*tex_ea;
	mlx_texture_t	*tex_we;
}	t_graphics;

typedef struct s_config
{
	char		*tex_no_ph;
	char		*tex_so_ph;
	char		*tex_ea_ph;
	char		*tex_we_ph;
	uint32_t	floor_color;
	uint32_t	sky_color;
}	t_config;

typedef struct s_map
{
	int		**world;
	size_t	width;
	size_t	height;
}	t_map;

typedef struct s_app
{
	t_player	player;
	t_graphics	gfx;
	t_config	config;
	t_map		map;
}	t_app;

typedef struct s_ray_data
{
	double	raydir_x;
	double	raydir_y;
	int		mp_x;
	int		mp_y;
	double	deltad_x;
	double	deltad_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
}	t_ray_data;

typedef struct s_column_draw
{
	int		x;
	int		draw_start;
	int		draw_end;
	int		line_h;
	int		side;
	double	raydir_x;
	double	raydir_y;
	double	wall_x;
}	t_column_draw;

typedef struct s_cube
{
	char	map[ROW + 1][COL + 1];
	char	no[COL + 1];
	char	so[COL + 1];
	char	ea[COL + 1];
	char	we[COL + 1];
	size_t	x_size;
	size_t	y_size;
	int		f[RGB];
	int		c[RGB];
}	t_cube;

typedef struct s_file
{
	t_cube		level;
}	t_file;

typedef struct s_file_elem
{
	int		fd;
	char	*line;
	t_cube	*src;
}	t_file_elem;

typedef struct s_tex_info
{
	int				tex_x;
	int				text_wi;
	int				text_heig;
	mlx_texture_t	*texture;
}	t_tex_info;

typedef struct s_step_info
{
	int		step;
	double	side_dist;
}	t_step_info;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_vertical_segment
{
	int			x;
	int			start_y;
	int			end_y;
	uint32_t	color;
}	t_vertical_segment;

void		draw_full_column(t_app *app, t_column_draw *col_draw,
				t_tex_info tex);
void		draw_wall(t_app *app, int x, t_column_draw *cd, t_tex_info tex);
void		render_column(t_app *app, int x);
void		init_ray_data(t_app *app, int x, t_ray_data *rd);
double		dda(t_app *app, t_ray_data *rd);
t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wall_x);

void		clear_image(mlx_image_t *img, uint32_t color);
void		draw_line(mlx_image_t *img, t_point p0, t_point p1, uint32_t color);
void		render_3d_view(t_app *app);
void		render_minimap(t_app *app);
void		update_frame(void *param);

void		parse_cub_data(t_app *app, t_file *cube);

void	process_input(t_app *app);
void	rotate_player(t_app *app, double angle);
void	move_player_foward_backward(t_app *app, double mult);
void	move_player_right_left(t_app *app, double mult);

void		cleanup_app(t_app *app);
void		msg_error(const char *error_msg, const int error_code);

int32_t		ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
char		*prepend_prefix(const char *prefix, const char *s);
char		*ensure_texture_path(const char *s);

void		convert(int argc, char *argv[], t_cube *src);
int			ft_error(const char *error_msg);
int			check_base(int argc, char *argv[]);
void		error_msg_setter(t_file_elem *file_el,
				const char *error_msg, const int error_code);
void		set_map(t_file_elem *file_el);
void		checker_map(t_cube *src);
void		get_elements(t_file_elem *file_el);
int			set_rgb(const char *str, int *dest);

void		set_player_direction(t_app *app, char c);
void		parse_textures(t_app *app, t_file *cube);
void		parse_colors(t_app *app, t_file *cube);
void		allocate_map(t_app *app, t_file *cube);
void		fill_map(t_app *app, t_file *cube);

#endif
