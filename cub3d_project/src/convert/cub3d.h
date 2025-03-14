/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 20:36:16 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 20:36:17 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../../includes/get_next_line.h"
# include "../../includes/app.h"

# define ERROR "\033[31mError!\033[0m"
# define ROW 100
# define COL 80

typedef struct s_cube
{
	char	map[ROW + 1][COL + 1];
	char	no[COL + 1];
	char	so[COL + 1];
	char	ea[COL + 1];
	char	we[COL + 1];
	size_t	x_size;
	size_t	y_size;
	int		f[3];
	int		c[3];
}	t_cube;

typedef struct s_file
{
	t_cube		level;
}	t_file;

void		convert(int argc, char *argv[], t_cube *src);
int			ft_error(const char *error_msg);

#endif
