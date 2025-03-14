/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:29:08 by bmoretti          #+#    #+#             */
/*   Updated: 2025/03/13 19:25:20 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORT_H
# define IMPORT_H

# include "cub3d.h"

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

typedef struct s_file_elem
{
	int		fd;
	char	*line;
	t_cube	*src;
}	t_file_elem;

int		check_base(int argc, char *argv[]);
void	error_msg_setter(t_file_elem *file_el,
			const char *error_msg, const int error_code);
void	set_map(t_file_elem *file_el);
void	checker_map(t_cube *src);
void	get_elements(t_file_elem *file_el);
int		set_rgb(const char *str, int *dest);

#endif
