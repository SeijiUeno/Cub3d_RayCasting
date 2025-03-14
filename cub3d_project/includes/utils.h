/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:40:39 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/13 16:13:39 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdint.h>

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
char	*prepend_prefix(const char *prefix, const char *s);
char	*ensure_texture_path(const char *s);

#endif
