/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:30 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:20:48 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_UTILS_H
# define RAYCAST_UTILS_H

# include "../../../includes/structures.h"
# include "../../../includes/raycasting.h"

# define RAY_INFINITY 1e30

typedef struct s_tex_info
{
	int				texX;
	int				texWidth;
	int				texHeight;
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

#endif
