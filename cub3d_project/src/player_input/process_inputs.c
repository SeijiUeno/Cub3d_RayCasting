/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_inputs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:06:03 by ecoelho-          #+#    #+#             */
/*   Updated: 2025/03/14 17:06:05 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void process_player_movement(t_app *app)
{
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_ESCAPE))
		mlx_close_window(app->gfx.mlx);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_W))
		move_player_foward_backward(app, 1.0);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_S))
		move_player_foward_backward(app, -1.0);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_D))
		move_player_right_left(app, 1.0);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_A))
		move_player_right_left(app, -1.0);
}

static void process_fixing_rotate(t_app *app)
{
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_LEFT))
		rotate_player(app, -app->player.rot_sped);
	if (mlx_is_key_down(app->gfx.mlx, MLX_KEY_RIGHT))
		rotate_player(app, app->player.rot_sped);
}
void	process_input(t_app *app)
{
	process_player_movement(app);
	process_fixing_rotate(app);
}
