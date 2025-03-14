/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:35:26 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:29:31 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

int	ft_error(const char *error_msg)
{
	ft_putendl_fd(ERROR, STDERR_FILENO);
	ft_putendl_fd((char *)error_msg, STDERR_FILENO);
	return (0);
}

void	msg_error(const char *error_msg, const int error_code)
{
	ft_error(error_msg);
	exit (error_code);
}

void	error_msg_setter(t_file_elem *file_el,
	const char *error_msg, const int error_code)
{
	char	*line;

	if (file_el)
		free(file_el->line);
	if (file_el->fd)
		close (file_el->fd);
	line = get_next_line(file_el->fd);
	while (line)
	{
		free (line);
		line = get_next_line(file_el->fd);
	}
	if (error_msg)
		msg_error(error_msg, error_code);
	exit (error_code);
}
