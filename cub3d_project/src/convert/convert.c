/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:36:06 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 20:57:21 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"

void	convert(int argc, char *argv[], t_cube *src)
{
	t_file_elem	file_el;

	src->x_size = 0;
	src->y_size = 0;
	file_el.fd = check_base(argc, argv);
	file_el.src = src;
	file_el.line = NULL;
	get_elements(&file_el);
	set_map(&file_el);
	checker_map(src);
	close(file_el.fd);
}
