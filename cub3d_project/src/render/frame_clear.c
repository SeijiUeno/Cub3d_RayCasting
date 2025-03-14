/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_clear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:05:20 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:43:41 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

void	clear_image(mlx_image_t *img, uint32_t color)
{
	size_t		total;
	size_t		i;
	uint32_t	*pixels;

	total = (size_t)img->width * img->height;
	i = 0;
	pixels = (uint32_t *)img->pixels;
	while (i < total)
	{
		pixels[i] = color;
		i = i + 1;
	}
}
