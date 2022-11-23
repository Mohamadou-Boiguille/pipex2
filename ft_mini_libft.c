#include "pipex.h"
#include <stdint.h>

int	ft_strncmp(const char *s1, const char *s2, size_t len)
{
	size_t	i;

	i = 0;
	if (!len || (!s1 && !s2))
		return (0);
	while (i < len)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (!s1[i])
			break ;
		i++;
	}
	return (0);
}

void	*ft_memcpy(void *d, const void *s, int l)
{
	char	*dest;

	dest = d;
	if (!d && !s)
		return (d);
	while (l--)
	{
		*(char *)d++ = *(char *)s++;
	}
	return (dest);
}

size_t	ft_strlen(char const *s)
{
	size_t	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}

void	*ft_bzero(void *s, int n)
{
	char	*src;
	int		i;

	i = 0;
	src = s;
	while (i < n)
		src[i++] = '\0';
	return (s);
}

void	*ft_calloc(size_t nb_of_memb, size_t size)
{
	void	*ptr;

	if (nb_of_memb == SIZE_MAX && size == SIZE_MAX)
		return (NULL);
	ptr = malloc(nb_of_memb * size);
	if (ptr == NULL)
		return (ptr);
	ft_bzero(ptr, nb_of_memb * size);
	return (ptr);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size)
	{
		while ((i < size - 1) && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = 0;
	}
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dlen;
	size_t	slen;
	size_t	j;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	j = 0;
	if (dlen < size)
	{
		while ((dlen + j < size - 1) && src[j])
		{
			dst[dlen + j] = src[j];
			j++;
		}
		dst[dlen + j] = '\0';
		return (dlen + slen);
	}
	return (slen + size);
}
