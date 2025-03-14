/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:36:20 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 21:10:44 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "convert.h"

int	check_base(int argc, char *argv[])
{
	size_t	len;
	int		fd;

	if (argc != 2)
		msg_error("Usage: ./cub3d <map_file.cub>", 1);
	len = ft_strlen(argv[1]);
	if (len < 5 || ft_strncmp(argv[1] + len - 4, ".cub", 4))
		msg_error("Expected a .cub file", 2);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		msg_error(strerror(errno), 3);
	return (fd);
}

int	set_rgb(const char *str, int *dest)
{
	size_t	i;
	size_t	zeros;
	char	*mov;
	int		number;

	zeros = 0;
	mov = (char *)str;
	while (ft_isspace(*mov))
		mov++;
	while (mov[zeros] == '0')
		zeros++;
	i = 0;
	while (ft_isdigit(mov[zeros + i]))
		i++;
	if (i + zeros == 0 || i > 3)
		return (0);
	number = ft_atoi(mov + zeros);
	if (0 <= number && number <= 255)
	{
		*dest = (unsigned char)number;
		return (1);
	}
	return (0);
}
