/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_elements.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:41 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 21:21:49 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

static void	get_rgb(int dest[3], const char *str, t_import_elements *lvl_el)
{
	int		index;
	int		i;

	if (!fill_valid_rgb_number(str, dest))
		get_elements_error(lvl_el, "Bad RGB specification", 7);
	i = -1;
	index = 0;
	while (str[++i] && str[i] != '\n')
	{
		if (ft_isdigit(str[i]) || ft_isspace(str[i]))
			continue ;
		if (str[i] == ',' && ++index)
		{
			if (index > 2)
				get_elements_error(lvl_el, "Bad RGB specification", 8);
			if (!fill_valid_rgb_number(str + i + 1, dest + index))
				get_elements_error(lvl_el, "Bad RGB specification", 9);
			continue ;
		}
		get_elements_error(lvl_el, "Bad RGB specification", 10);
	}
	if (index == 2)
		return ;
	get_elements_error(lvl_el, "Bad RGB specification", 11);
}

static void	copy_element(int index, t_import_elements *lvl_el, const char *str)
{
	t_level	*lvl;

	lvl = lvl_el->lvl;
	if (index == NO)
		ft_memcpy(lvl->no, str, ft_strlen(str) - 1);
	else if (index == SO)
		ft_memcpy(lvl->so, str, ft_strlen(str) - 1);
	else if (index == WE)
		ft_memcpy(lvl->we, str, ft_strlen(str) - 1);
	else if (index == EA)
		ft_memcpy(lvl->ea, str, ft_strlen(str) - 1);
	else if (index == F)
		get_rgb(lvl->f, str, lvl_el);
	else if (index == C)
		get_rgb(lvl->c, str, lvl_el);
}

static void	get_element(t_import_elements *lvl_el, int *gotten_elements)
{
	const char	*elements[] = {"NO ", "SO ", "WE ", "EA ", "F ", "C ", NULL};
	short int	i;
	size_t		j;
	int			index;

	i = -1;
	while (elements[++i])
	{
		j = ft_strlen(elements[i]);
		if (!ft_strncmp(elements[i], lvl_el->line, j))
		{
			while (ft_isspace(lvl_el->line[j]))
				j++;
			index = 1 << i;
			if (*gotten_elements & index)
				get_elements_error(lvl_el, "Repeated element", 6);
			copy_element(index, lvl_el, lvl_el->line + j);
			*gotten_elements |= index;
			return ;
		}
	}
	get_elements_error(lvl_el, "Invalid or missing element", 8);
}

void	get_elements(t_import_elements *lvl_el)
{
	int		gotten_elements;

	gotten_elements = 0;
	while (gotten_elements != E_ALL)
	{
		lvl_el->line = get_next_line(lvl_el->fd);
		if (!lvl_el->line)
			get_elements_error(lvl_el, "Missing elements", 4);
		if (ft_strlen(lvl_el->line) > MAX_COLS)
			get_elements_error(lvl_el, "MAX_COLS exceeded", 5);
		if (*lvl_el->line != '\n')
			get_element(lvl_el, &gotten_elements);
		free (lvl_el->line);
	}
	lvl_el->line = NULL;
}
