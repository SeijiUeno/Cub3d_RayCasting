/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_3d_view.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:13 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:06:34 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include "static_includes/raycast_utils.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void	render_3d_view(t_app *app)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		render_column(app, x);
		x = x + 1;
	}
}
