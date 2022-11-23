#ifndef PIPEX_H
# define PIPEX_H

#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int		is_input_valid(int len);
void	*ft_memcpy(void *d, const void *s, int l);
char	**ft_split_and_add_file(char const *s, char c, char *file);
char	*ft_str_realloc(char* ptr, size_t size);
void	*ft_calloc(size_t nb_of_memb, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlen(char const *s);
#endif
