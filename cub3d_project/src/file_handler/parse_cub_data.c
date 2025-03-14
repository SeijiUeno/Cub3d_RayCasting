/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:42:32 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "helper/parser_helpers.h"

void	parse_cub_data(t_app *app, t_file *cube)
{
	parse_textures(app, cube);
	parse_colors(app, cube);
	allocate_map(app, cube);
	fill_map(app, cube);
}
