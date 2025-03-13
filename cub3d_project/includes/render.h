/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:40:29 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:19:43 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "app.h"
# include "../src/render/static_includes/raycast_utils.h"

# define WIDTH 1280
# define HEIGHT 720
# define CLEAR_COLOR 0x000000FF
# define MINIMAP_SCALE 6

void	clear_image(mlx_image_t *img, uint32_t color);
void	draw_line(mlx_image_t *img, t_point p0, t_point p1, uint32_t color);
void	render_3d_view(t_app *app);
void	render_minimap(t_app *app);
void	update_frame(void *param);

#endif
