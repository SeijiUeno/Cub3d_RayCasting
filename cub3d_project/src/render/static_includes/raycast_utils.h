#ifndef RAYCAST_UTILS_H
# define RAYCAST_UTILS_H

# include "../../../includes/structures.h"

# define RAY_INFINITY 1e30

typedef struct s_step_info
{
	int		step;
	double	sideDist;
}	t_step_info;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

/* Initializes ray data for a given column */
void		draw_wall(t_app *app, int x, t_column_draw *cd, t_tex_info tex);
void		render_column(t_app *app, int x);
void		init_ray_data(t_app *app, int x, t_ray_data *rd);
double		dda(t_app *app, t_ray_data *rd);
t_tex_info	select_tex_info(t_app *app, t_ray_data *rd, double wallX);

#endif
