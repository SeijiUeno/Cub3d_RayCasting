#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdint.h>

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
int		ft_atoi(const char *str);
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);

/* New helper prototypes */
char	*prepend_prefix(const char *prefix, const char *s);
char	*ensure_texture_path(const char *s);

#endif
