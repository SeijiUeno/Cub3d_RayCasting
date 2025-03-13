/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:11:48 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:14:17 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCASTING_H
# define RAYCASTING_H

typedef struct s_ray_data
{
	double	rayDirX;
	double	rayDirY;
	int		mapX;
	int		mapY;
	double	deltaDistX;
	double	deltaDistY;
	double	side_distX;
	double	side_distY;
	int		stepX;
	int		stepY;
	int		side;
}	t_ray_data;

typedef struct s_column_draw {
	int		x;
	int		drawStart;
	int		drawEnd;
	int		lineHeight;
	int		side;
	double	rayDirX;
	double	rayDirY;
	double	wallX;
}	t_column_draw;

#endif