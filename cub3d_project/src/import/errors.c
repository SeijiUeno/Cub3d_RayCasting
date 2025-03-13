/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:26 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 18:35:33 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

int	error_message(const char *error_msg)
{
	ft_putendl_fd(ERROR, STDERR_FILENO);
	ft_putendl_fd((char *)error_msg, STDERR_FILENO);
	return (0);
}

void	exit_error_message(const char *error_msg, const int error_code)
{
	error_message(error_msg);
	exit (error_code);
}

void	get_elements_error(t_import_elements *lvl_el,
	const char *error_msg, const int error_code)
{
	char	*line;

	if (lvl_el)
		free(lvl_el->line);
	if (lvl_el->fd)
		close (lvl_el->fd);
	line = get_next_line(lvl_el->fd);
	while (line)
	{
		free (line);
		line = get_next_line(lvl_el->fd);
	}
	if (error_msg)
		exit_error_message(error_msg, error_code);
	exit (error_code);
}
