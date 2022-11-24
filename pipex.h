#ifndef PIPEX_H
# define PIPEX_H

#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
# define IN 0
# define OUT 1
# define EXTRA_SPACE_AND_END_OF_STR 2

typedef struct s_inputs
{
  char  **path;
  char  **input_array;
  char  **first_cmd;
  char  **last_cmd;
  char  *first_cmd_with_path;
  char  *last_cmd_with_path;
  int   *fd_pipe;
  char  **envp;
  int   there_is_error;
  int   len;
}         t_inputs;

int		is_input_valid(int len);
int	ft_strncmp(const char *s1, const char *s2, size_t len);
void	*ft_memcpy(void *d, const void *s, int l);
char  **ft_split(char const *s, char c);
void	*ft_calloc(size_t nb_of_memb, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlen(char const *s);
#endif
