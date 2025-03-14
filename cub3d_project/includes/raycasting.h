/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:11:48 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 19:49:45 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTING_H
# define RAYCASTING_H

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

#endif