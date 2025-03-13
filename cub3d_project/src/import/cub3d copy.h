/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:13 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:35:17 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include "../../includes/get_next_line.h"
# include "../../includes/structures.h"

# define MAX_ROWS 100
# define MAX_COLS 80

typedef struct s_level
{
	char	no[MAX_COLS + 1];
	char	so[MAX_COLS + 1];
	char	ea[MAX_COLS + 1];
	char	we[MAX_COLS + 1];
	int		f[3];
	int		c[3];
	char	map[MAX_ROWS + 1][MAX_COLS + 1];
	size_t	x_size;
	size_t	y_size;
}	t_level;

typedef struct s_cub
{
	t_level		level;
}	t_cub;

void		import(int argc, char *argv[], t_level *lvl);
int			error_message(const char *error_msg);

#endif
