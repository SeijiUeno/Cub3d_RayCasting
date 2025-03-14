/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_maps.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:36:12 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:36:13 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"

static void	check_wall(t_cube *src, int start[2], int i_step, int j_step)
{
	int	i;
	int	j;

	i = start[0] + i_step;
	j = start[1] + j_step;
	while (i > -1 && j > -1 && src->map[i][j]
		&& src->map[i][j] != ' ')
	{
		if (src->map[i][j] == '1')
			return ;
		if (src->map[i][j] == ' ')
			break ;
		i += i_step;
		j += j_step;
	}
	msg_error("error", 33);
}

static void	walls_around(t_cube *src, int i, int j)
{
	int	start[2];

	if (ft_strchr(" 1", src->map[i][j]))
		return ;
	start[0] = i;
	start[1] = j;
	check_wall(src, start, -1, 0);
	check_wall(src, start, 1, 0);
	check_wall(src, start, 0, -1);
	check_wall(src, start, 0, 1);
}

static void	valid_char(const char c, int *count_player)
{
	const char	valids[4] = " 01";
	const char	player[5] = "NSEW";

	if (ft_strchr(valids, c))
		return ;
	if (!ft_strchr(player, c))
		msg_error("Bad map charachter", 34);
	(*count_player)++;
	if (*count_player > 1)
		msg_error("Only one player allowed in the map", 30);
}

void	checker_map(t_cube *src)
{
	int	i;
	int	j;
	int	count_player;

	count_player = 0;
	i = -1;
	while (++i < ROW && src->map[i][0])
	{
		j = -1;
		while (++j < COL && src->map[i][j])
		{
			valid_char(src->map[i][j], &count_player);
			walls_around(src, i, j);
		}
	}
	if (!count_player)
		msg_error("One player needed in the map", 31);
}
