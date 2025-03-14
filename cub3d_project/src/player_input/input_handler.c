/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecoelho- <ecoelho-@student.42>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:43:02 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 17:13:27 by ecoelho-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void	normalize_vector(double *x, double *y)
{
	double	len;

	len = sqrt((*x) * (*x) + (*y) * (*y));
	if (len != 0)
	{
		*x /= len;
		*y /= len;
	}
}

static void	rotate_vector(double *x, double *y, double cos_a, double sin_a)
{
	double	old_x;

	old_x = *x;
	*x = (*x * cos_a) - (*y * sin_a);
	*y = old_x * sin_a + (*y * cos_a);
}

void	rotate_player(t_app *app, double angle)
{
	double	cos_a;
	double	sin_a;

	cos_a = cos(angle);
	sin_a = sin(angle);
	rotate_vector(&app->player.dir_x, &app->player.dir_y, cos_a, sin_a);
	rotate_vector(&app->player.plan_x, &app->player.plan_y, cos_a, sin_a);
	normalize_vector(&app->player.dir_x, &app->player.dir_y);
	normalize_vector(&app->player.plan_x, &app->player.plan_y);
	app->player.plan_x *= 0.66;
	app->player.plan_y *= 0.66;
}

void	move_player_foward_backward(t_app *app, double mult)
{
	double	new_x;
	double	new_y;
	double	move_x;
	double	move_y;

	move_x = app->player.dir_x * app->player.move_sped * mult;
	move_y = app->player.dir_y * app->player.move_sped * mult;
	new_x = app->player.pos_x + move_x;
	new_y = app->player.pos_y + move_y;
	if (new_x >= 0 && new_x < app->map.width
		&& app->map.world[(int)app->player.pos_y][(int)new_x] == 0)
		app->player.pos_x = new_x;
	if (new_y >= 0 && new_y < app->map.height
		&& app->map.world[(int)new_y][(int)app->player.pos_x] == 0)
		app->player.pos_y = new_y;
}

void	move_player_right_left(t_app *app, double mult)
{
	double	new_x;
	double	new_y;
	double	move_x;
	double	move_y;

	move_x = app->player.plan_x * app->player.move_sped * mult;
	move_y = app->player.plan_y * app->player.move_sped * mult;
	new_x = app->player.pos_x + move_x;
	new_y = app->player.pos_y + move_y;
	if (new_x >= 0 && new_x < app->map.width
		&& app->map.world[(int)app->player.pos_y][(int)new_x] == 0)
		app->player.pos_x = new_x;
	if (new_y >= 0 && new_y < app->map.height
		&& app->map.world[(int)new_y][(int)app->player.pos_x] == 0)
		app->player.pos_y = new_y;
}


