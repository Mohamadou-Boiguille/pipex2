#ifndef PIPEX_H
#define PIPEX_H

#include "/libft/libft.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#define EXTRA_SPACE_AND_END_OF_STR 2

typedef struct s_inputs
{
	char **path;
	char **input_array;
	char **first_cmd;
	char **last_cmd;
	char * first_cmd_with_path;
	char * last_cmd_with_path;
	int *  fd_pipe;
	char **envp;
	int    there_is_error;
	int    len;
} t_inputs;

int *     ft_create_pipe(void);
void      ft_close_fds(t_inputs *input_set);
void      ft_free_splited_arrays(char **str_arr);
void      check_valid_nb_of_args(int nb_of_args);
int       ft_find_nb_of_words(char *str);
char **   ft_input_to_shell_format(char *cmd_and_args, char *filename);
char **   ft_get_path_array(char **env_varibles);
int       ft_check_if_infile_exist_and_access(char *filename);
int       ft_check_outfile_access(char *filename);
void      ft_free_struct(t_inputs *inputs);
char *    create_cmd_with_path(char **path, char *cmd);
char *    create_filename_with_path(char **path, char *filename);
int       ft_is_cmds_executables(t_inputs *input_set);
t_inputs *init_input_set(int len, char **input, char **envp);
void      ft_child_process(t_inputs *input);
void      ft_parent_process(t_inputs *input);
int       is_input_valid(int len);
int       ft_strncmp(const char *s1, const char *s2, size_t len);
void *    ft_memcpy(void *d, const void *s, int l);
char **   ft_split(char const *s, char c);
void *    ft_calloc(size_t nb_of_memb, size_t size);
size_t    ft_strlcpy(char *dst, const char *src, size_t size);
size_t    ft_strlcat(char *dst, const char *src, size_t size);
size_t    ft_strlen(char const *s);
#endif
