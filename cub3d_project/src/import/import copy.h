/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:29:08 by bmoretti          #+#    #+#             */
/*   Updated: 2025/03/13 17:36:06 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORT_H
# define IMPORT_H

# include "cub3d.h"

# define ERROR "\033[31mError\033[0m"

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

typedef struct s_import_elements
{
	int		fd;
	char	*line;
	t_level	*lvl;
}	t_import_elements;

int		basic_validation(int argc, char *argv[]);

void	exit_error_message(const char *error_msg, const int error_code);
void	get_elements_error(t_import_elements *lvl_el,
			const char *error_msg, const int error_code);

void	get_elements(t_import_elements *lvl_el);
int		fill_valid_rgb_number(const char *str, int *dest);

void	get_map(t_import_elements *lvl_el);
void	map_validation(t_level *lvl);

#endif
