#include "pipex.h"
#include <string.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int *ft_create_pipe(void)
{
  int *fd;

  fd = malloc(2 * sizeof(int));
  if (!fd)
    return (NULL);
  if(pipe(fd) == -1)
  {
    exit(127);
    fprintf(stderr, "Pipe error : %s\n", strerror(errno));
  }
  return (fd);
}

void  ft_close_all_fd(t_inputs *input_set)
{
  close(input_set->fd_pipe[0]);
  close(input_set->fd_pipe[1]);
}

void	ft_free_splited_arrays(char **str_arr)
{
	int	i;

	i = 0;
	if (!str_arr)
		return ;
	while (str_arr[i] != NULL)
		free(str_arr[i++]);
	free(str_arr);
}

void check_valid_nb_of_args(int nb_of_args)
{
	if (nb_of_args != 5)
	{
    fprintf(stderr, "Mauvais nombre d'arguments : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

char	*create_cmd_str_with_path(t_inputs *input_set, char *cmd)
{
  int   i;
  int   len;
	char	*cmd_with_path;

  i = 0;
  if (!access(cmd, X_OK))
    return(cmd);
  while (input_set->path[i])
  {
    len = ft_strlen(cmd) + ft_strlen(input_set->path[i]) + 2; 
    cmd_with_path = ft_calloc(len, sizeof(char));
    ft_strlcat(cmd_with_path, input_set->path[i], len);
    ft_strlcat(cmd_with_path, "/", len);
    ft_strlcat(cmd_with_path, cmd, len);
    if (!access(cmd_with_path, X_OK))
      return (cmd_with_path);
    free(cmd_with_path);
    i++;
  }
	return (NULL);
}

int ft_error_handler(char *message, int code, t_inputs *inputs)
{
  if (inputs)
  {
    if (inputs->cmd_with_path)
      free(inputs->cmd_with_path);
    free(inputs->fd_pipe);
    ft_free_splited_arrays(inputs->first_cmd);
    ft_free_splited_arrays(inputs->last_cmd);
    ft_free_splited_arrays(inputs->path);
  }    
  fprintf(stderr, "%s : %s\n", message, strerror(errno));
  exit(code);
}

void  ft_execute_cmd(t_inputs *input_set, char **cmd_array)
{
	input_set->cmd_with_path = create_cmd_str_with_path(input_set, cmd_array[0]);
  if (input_set->cmd_with_path == NULL)
    ft_error_handler("MY_command not found", 127, input_set);
	if (execve(input_set->cmd_with_path, cmd_array, NULL) == -1)
  {
    ft_error_handler("EXECUTION ERROR", 127, input_set);
  }

}

int ft_find_nb_of_words(char *str)
{
  size_t  i;
  int     nb_of_words;

  i = 0;
  nb_of_words = 1;
  while (str[i])
  {
    if (str[i] == ' ')
      nb_of_words++;
    i++;
  }
  return (nb_of_words);
}

char  **ft_input_to_shell_format(char *cmd_and_args, char *filename)
{
  int   cmd_array_size;
  char  *shell_format_cmd;
  char  **cmd_array;
  int   str_size;

  str_size = EXTRA_SPACE_AND_END_OF_STR + ft_strlen(cmd_and_args);
  if (filename)
    str_size += ft_strlen(filename);
  shell_format_cmd = ft_calloc(str_size, sizeof(char));
  ft_strlcat(shell_format_cmd, cmd_and_args, str_size);
  if (filename)
  {
    ft_strlcat(shell_format_cmd, " ", str_size);
    ft_strlcat(shell_format_cmd, filename, str_size);
  }
  cmd_array = ft_split(shell_format_cmd, ' ');
  free(shell_format_cmd);
  return (cmd_array);
}

void ft_child_process(t_inputs *input)
{
  dup2(input->fd_pipe[1], STDOUT_FILENO);
  ft_close_all_fd(input);
  ft_execute_cmd(input, input->first_cmd);
}

void  ft_parent_process(t_inputs *input)
{
  int file_fd;

  file_fd = open(input->input_array[4], O_CREAT | O_TRUNC | O_WRONLY, 0666);
  if (file_fd == -1)
    ft_error_handler("file opening error", errno, input);
  dup2(file_fd, STDOUT_FILENO);
  dup2(input->fd_pipe[0], STDIN_FILENO);
  ft_close_all_fd(input);
  ft_execute_cmd(input, input->last_cmd);
}

void  ft_check_if_infile_exist_and_access(char *filename)
{
  if (access(filename, F_OK) == -1)
  {
    fprintf(stderr, "%s : Fichier inexistant - %s(%d)\n", filename, strerror(errno), errno);
    exit(errno);
  }
  if (access(filename, R_OK) == -1)
  {
    fprintf(stderr, "%s : Erreur acces lecture - %s\n", filename, strerror(errno));
    exit(errno);
  }
}

void  ft_check_outfile_access(char *filename)
{
  if (access(filename, F_OK) == 0 && access(filename, W_OK) == -1)
  {
    fprintf(stderr, "%s : Fichier inexistant - %s\n", filename, strerror(errno));
    exit(126);
  }
}

char **ft_get_path_array(char **env_varibles)
{
  int i = 0;
  char **path_array;
  char *path_str;

  path_array = NULL;
  while(path_array == NULL && env_varibles[i] != NULL)
  {
    if (!ft_strncmp("PATH=", env_varibles[i], 5))
      path_array = ft_split(env_varibles[i] + 5, ':');
    i++;
  }
  return (path_array);
}

void ft_is_inputs_valid(int len, char **input_array)
{
	check_valid_nb_of_args(len);
  ft_check_if_infile_exist_and_access(input_array[1]);
  ft_check_outfile_access(input_array[len- 1]);
}

char *ft_last_cmd_status_ptr(char **envp)
{
  int   i;

  i = 0;
  while (envp[i])
  {
    if (envp[i][0] == '?')
      return (&envp[i][2]);
    i++;
  }
  return (NULL);
}

int ft_exit_on_err(char *message, int code, t_inputs *inputs)
{
  if (inputs)
  {
    if (inputs->cmd_with_path)
      free(inputs->cmd_with_path);
    free(inputs->fd_pipe);
    ft_free_splited_arrays(inputs->first_cmd);
    ft_free_splited_arrays(inputs->last_cmd);
    ft_free_splited_arrays(inputs->path);
  }    
  fprintf(stderr, "%s : %s\n", message, strerror(errno));
  exit(code);
}

t_inputs  *init_input_set(int len, char **input, char **envp)
{
  t_inputs  *input_set;

  input_set = malloc(sizeof(t_inputs));
  if (!input_set)
    return (NULL);
  input_set->input_array = input;
  input_set->first_cmd = ft_input_to_shell_format(input[2], input[1]);
  input_set->last_cmd = ft_input_to_shell_format(input[len - 2], NULL);
  input_set->path = ft_get_path_array(envp);
  input_set->fd_pipe = ft_create_pipe();
  input_set->envp = envp;
  input_set->cmd_with_path = NULL;
  input_set->len = len;
  return (input_set);
}

int   ft_process_error(int *error_pipe)
{
  char *is_child_error;

  read(error_pipe[0], &is_child_error, 1);
  return (*is_child_error);
}

int	main(int argc, char **argv, char **envp)
{
	int	pid1;
  t_inputs *input_set;
  errno = 0;

  ft_is_inputs_valid(argc, argv);
  input_set = init_input_set(argc, argv, envp);
	pid1 = fork();
	if (pid1 == -1)
    return ft_error_handler("FORK ERROR", -1, input_set);
	if (pid1 == 0 && !errno)
    ft_child_process(input_set);
	if (pid1 != 0 && input_set->last_error_code_ptr == 0)
	{
    wait(NULL);
    ft_is_inputs_valid(argc, argv);
    ft_parent_process(input_set);
	}
  ft_close_all_fd(input_set);
	return (errno);
}
