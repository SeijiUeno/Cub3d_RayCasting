/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brmoretti <brmoretti@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 11:43:12 by brmoretti         #+#    #+#             */
/*   Updated: 2024/03/22 13:24:34 by brmoretti        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "import.h"

int	basic_validation(int argc, char *argv[])
{
	size_t	len;
	int		fd;

	if (argc != 2)
		exit_error_message("Usage: ./cub3d <map_file.cub>", 1);
	len = ft_strlen(argv[1]);
	if (len < 5 || ft_strncmp(argv[1] + len - 4, ".cub", 4))
		exit_error_message("Expected a .cub file", 2);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		exit_error_message(strerror(errno), 3);
	return (fd);
}

int	fill_valid_rgb_number(const char *str, int *dest)
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
