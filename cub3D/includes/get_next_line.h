/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sueno-te <sueno-te@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 13:54:21 by sueno-te          #+#    #+#             */
/*   Updated: 2025/03/14 15:54:33 by sueno-te         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdint.h>
# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>

# ifndef BUFFER
#  define BUFFER 1024
# endif

typedef struct s_bffer
{
	char	bffer[BUFFER + 1];
	size_t	pos;
	int		bslash;
}	t_bffer;

void	*ft_memset(void *s, int c, size_t n);
int		ft_isdigit(int c);
int		ft_isspace(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_bzero(void *s, size_t n);
char	*get_next_line(int fd);
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	*ft_strndup(const char *s, size_t n);
int		ft_atoi(const char *str);
char	*ft_strchr(const char *s, int c);

#endif
