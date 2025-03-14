/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:36:18 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:52:35 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <MLX42/MLX42.h>
# include <stdint.h>

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

#endif