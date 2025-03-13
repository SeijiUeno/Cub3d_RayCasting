/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmoretti <brmoretti@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:25 by brmoretti         #+#    #+#             */
/*   Updated: 2024/03/26 20:28:43 by brmoretti        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

static void	check_wall(t_level *lvl, int start[2], int i_step, int j_step)
{
	int	i;
	int	j;

	i = start[0] + i_step;
	j = start[1] + j_step;
	while (i > -1 && j > -1 && lvl->map[i][j]
		&& lvl->map[i][j] != ' ')
	{
		if (lvl->map[i][j] == '1')
			return ;
		if (lvl->map[i][j] == ' ')
			break ;
		i += i_step;
		j += j_step;
	}
	exit_error_message("🎵 We don't need no education...\n"
		"🧱 Missing another brick in the wall", 33);
}

static void	walls_around(t_level *lvl, int i, int j)
{
	int	start[2];

	if (ft_strchr(" 1", lvl->map[i][j]))
		return ;
	start[0] = i;
	start[1] = j;
	check_wall(lvl, start, -1, 0);
	check_wall(lvl, start, 1, 0);
	check_wall(lvl, start, 0, -1);
	check_wall(lvl, start, 0, 1);
}

static void	valid_char(const char c, int *count_player)
{
	const char	valids[4] = " 01";
	const char	player[5] = "NSEW";

	if (ft_strchr(valids, c))
		return ;
	if (!ft_strchr(player, c))
		exit_error_message("Bad map charachter", 34);
	(*count_player)++;
	if (*count_player > 1)
		exit_error_message("Only one player allowed in the map", 30);
}

void	map_validation(t_level *lvl)
{
	int	i;
	int	j;
	int	count_player;

	count_player = 0;
	i = -1;
	while (++i < MAX_ROWS && lvl->map[i][0])
	{
		j = -1;
		while (++j < MAX_COLS && lvl->map[i][j])
		{
			valid_char(lvl->map[i][j], &count_player);
			walls_around(lvl, i, j);
		}
	}
	if (!count_player)
		exit_error_message("One player needed in the map", 31);
}
