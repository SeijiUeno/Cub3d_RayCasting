#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdint.h>

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strndup(const char *s, size_t n);
int		ft_atoi(const char *str);
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a);

/* New helper to ensure texture paths start with "assets/textures/" */
char	*prepend_prefix(const char *prefix, const char *s);
char	*ensure_texture_path(const char *s);

#endif
