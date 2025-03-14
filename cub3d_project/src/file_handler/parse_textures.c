/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:40:42 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:50:51 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"
#include "../../includes/file_utils.h"
#include "../../includes/utils.h"
#include "../convert/cub3d.h"

static char	*get_tex_path(const char *prefix, const char *path)
{
	if (ft_strncmp(path, prefix, ft_strlen(prefix)) == 0)
		return (ft_strdup(path));
	return (prepend_prefix(prefix, path));
}

void	parse_textures(t_app *app, t_file *cube)
{
	const char	*prefix = "assets/textures/";

	app->config.tex_no_ph = get_tex_path(prefix, cube->level.no);
	app->config.tex_so_ph = get_tex_path(prefix, cube->level.so);
	app->config.tex_ea_ph = get_tex_path(prefix, cube->level.ea);
	app->config.tex_we_ph = get_tex_path(prefix, cube->level.we);
}
