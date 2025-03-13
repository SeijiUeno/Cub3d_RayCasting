/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:18 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:06:09 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "../../includes/input_handler.h"

void	update_frame(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	process_input(app);
	clear_image(app->gfx.img, CLEAR_COLOR);
	render_3d_view(app);
	render_minimap(app);
}
