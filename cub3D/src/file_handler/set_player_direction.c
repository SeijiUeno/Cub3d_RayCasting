/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_player_direction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:24:33 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/17 10:25:45 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

static void	set_north(t_app *app)
{
	app->player.dir_x = 0;
	app->player.dir_y = 1;
	app->player.plan_x = 0.66;
	app->player.plan_y = 0;
}

static void	set_south(t_app *app)
{
	app->player.dir_x = 0;
	app->player.dir_y = -1;
	app->player.plan_x = -0.66;
	app->player.plan_y = 0;
}

static void	set_east(t_app *app)
{
	app->player.dir_x = -1;
	app->player.dir_y = 0;
	app->player.plan_x = 0;
	app->player.plan_y = 0.66;
}

static void	set_west(t_app *app)
{
	app->player.dir_x = 1;
	app->player.dir_y = 0;
	app->player.plan_x = 0;
	app->player.plan_y = -0.66;
}

void	set_player_direction(t_app *app, char c)
{
	if (c == 'N')
		set_north(app);
	else if (c == 'S')
		set_south(app);
	else if (c == 'E')
		set_east(app);
	else if (c == 'W')
		set_west(app);
}
