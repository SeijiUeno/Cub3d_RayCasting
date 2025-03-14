/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:44:48 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HELPERS_H
# define PARSER_HELPERS_H

# include "../../convert/cub3d.h"

void	parse_textures(t_app *app, t_file *cube);
void	parse_colors(t_app *app, t_file *cube);
void	allocate_map(t_app *app, t_file *cube);
void	fill_map(t_app *app, t_file *cube);
void	set_player_direction(t_app *app, char c);

#endif
