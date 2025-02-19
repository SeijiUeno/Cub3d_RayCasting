#include "../includes/utils.h"
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup(const char *s)
{
	size_t	len = ft_strlen(s);
	char	*dup = malloc(len + 1);
	size_t	i;

	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_atoi(const char *str)
{
	int	sign = 1;
	int	res = 0;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	dup = malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

/* Prepend a prefix to s and return the new allocated string */
char	*prepend_prefix(const char *prefix, const char *s)
{
	size_t	len_prefix = ft_strlen(prefix);
	size_t	len_s = ft_strlen(s);
	char	*result;
	size_t	i, j;

	result = malloc(len_prefix + len_s + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (prefix[i])
	{
		result[i] = prefix[i];
		i++;
	}
	j = 0;
	while (s[j])
	{
		result[i + j] = s[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

/* Ensure that the texture path begins with "assets/textures/" */
char	*ensure_texture_path(const char *s)
{
	const char	*prefix = "assets/textures/";
	size_t		prefix_len = ft_strlen(prefix);

	if (ft_strncmp(s, prefix, prefix_len) == 0)
		return (ft_strdup(s));
	return (prepend_prefix(prefix, s));
}