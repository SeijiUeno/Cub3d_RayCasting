/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:36:20 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 16:36:43 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/app.h"

int	check_base(int argc, char *argv[])
{
	size_t	len;
	int		fd;

	(void)argc;
	len = ft_strlen(argv[1]);
	if (len < 5 || ft_strncmp(argv[1] + len - 4, ".cub", 42))
		msg_error("The map should be a .cub file", 42);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		msg_error("ERROR to open the file!", 42);
	return (fd);
}
