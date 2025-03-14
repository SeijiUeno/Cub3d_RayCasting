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

#include "../../includes/app.h"

static void	get_rgb(int dest[3], const char *str, t_file_elem *file_el)
{
	int		index;
	int		i;

	if (!set_rgb(str, dest))
		error_msg_setter(file_el, "FIle Bad RGB", 7);
	i = -1;
	index = 0;
	while (str[++i] && str[i] != '\n')
	{
		if (ft_isdigit(str[i]) || ft_isspace(str[i]))
			continue ;
		if (str[i] == ',' && ++index)
		{
			if (index > 2)
				error_msg_setter(file_el, "FIle Bad RGB", 8);
			if (!set_rgb(str + i + 1, dest + index))
				error_msg_setter(file_el, "FIle Bad RGB", 9);
			continue ;
		}
		error_msg_setter(file_el, "FIle Bad RGB!!!", 10);
	}
	if (index == 2)
		return ;
	error_msg_setter(file_el, "FIle Bad RGB!!", 11);
}

static void	copy_element(int index, t_file_elem *file_el, const char *str)
{
	t_cube	*src;

	src = file_el->src;
	if (index == NO)
		ft_memcpy(src->no, str, ft_strlen(str) - 1);
	else if (index == SO)
		ft_memcpy(src->so, str, ft_strlen(str) - 1);
	else if (index == WE)
		ft_memcpy(src->we, str, ft_strlen(str) - 1);
	else if (index == EA)
		ft_memcpy(src->ea, str, ft_strlen(str) - 1);
	else if (index == F)
		get_rgb(src->f, str, file_el);
	else if (index == C)
		get_rgb(src->c, str, file_el);
}

static void	get_element(t_file_elem *file_el, int *gotten_elements)
{
	short int	i;
	size_t		j;
	int			index;
	const char	*elements[] = {"NO ", "SO ", "WE ", "EA ", "F ", "C ", NULL};

	i = -1;
	while (elements[++i])
	{
		j = ft_strlen(elements[i]);
		if (!ft_strncmp(elements[i], file_el->line, j))
		{
			while (ft_isspace(file_el->line[j]))
				j++;
			index = 1 << i;
			if (*gotten_elements & index)
				error_msg_setter(file_el, "RePeTeD Elements!!", 6);
			copy_element(index, file_el, file_el->line + j);
			*gotten_elements |= index;
			return ;
		}
	}
	error_msg_setter(file_el, "missing element or invalid one", 8);
}

void	get_elements(t_file_elem *file_el)
{
	int		gotten_elements;

	gotten_elements = 0;
	while (gotten_elements != E_ALL)
	{
		file_el->line = get_next_line(file_el->fd);
		if (!file_el->line)
			error_msg_setter(file_el, "Missing Elements In the Cub File", 4);
		if (ft_strlen(file_el->line) > COL)
			error_msg_setter(file_el, "COL ERROR - TO MANY", 5);
		if (*file_el->line != '\n')
			get_element(file_el, &gotten_elements);
		free (file_el->line);
	}
	file_el->line = NULL;
}
