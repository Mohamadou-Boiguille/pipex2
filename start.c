#include "pipex.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void ft_exit_on_err(char *message, int code, t_inputs *inputs);

int *ft_create_pipe(void)
{
	int *fd;

	fd = malloc(2 * sizeof(int));
	if (!fd)
		return (NULL);
	if (pipe(fd) == -1)
	{
		exit(127);
		fprintf(stderr, "Pipe error : %s\n", strerror(errno));
	}
	return (fd);
}

void ft_close_fds(t_inputs *input_set)
{
	close(input_set->fd_pipe[0]);
	close(input_set->fd_pipe[1]);
}

void ft_free_splited_arrays(char **str_arr)
{
	int i;

	i = 0;
	if (!str_arr)
		return;
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

char *create_cmd_with_path(char **path, char *cmd)
{
	int   i;
	int   len;
	char *cmd_with_path;

	i = 0;
	if (!access(cmd, X_OK))
		return (cmd);
	while (path[i])
	{
		len = ft_strlen(cmd) + ft_strlen(path[i]) + 2;
		cmd_with_path = ft_calloc(len, sizeof(char));
		if (cmd_with_path == NULL)
			break;
		ft_strlcat(cmd_with_path, path[i], len);
		ft_strlcat(cmd_with_path, "/", len);
		ft_strlcat(cmd_with_path, cmd, len);
		if (!access(cmd_with_path, X_OK))
			return (cmd_with_path);
		free(cmd_with_path);
		i++;
	}
	return (NULL);
}

char *create_filename_with_path(char **path, char *filename)
{
	int   i;
	int   len;
	char *filename_with_path;

	i = 0;
	if (!access(filename, R_OK))
		return (filename);
	while (path[i])
	{
		len = ft_strlen(filename) + ft_strlen(path[i]) + 2;
		filename_with_path = ft_calloc(len, sizeof(char));
		if (filename_with_path == NULL)
			break;
		ft_strlcat(filename_with_path, path[i], len);
		ft_strlcat(filename_with_path, "/", len);
		ft_strlcat(filename_with_path, filename, len);
		if (!access(filename_with_path, X_OK))
			return (filename_with_path);
		free(filename_with_path);
		i++;
	}
	perror(filename);
	return (NULL);
}

int ft_find_nb_of_words(char *str)
{
	size_t i;
	int    nb_of_words;

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

char **ft_input_to_shell_format(char *cmd_and_args, char *filename)
{
	int    cmd_array_size;
	char  *shell_format_cmd;
	char **cmd_array;
	int    str_size;

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

char **ft_get_path_array(char **env_varibles)
{
	int    i = 0;
	char **path_array;
	char  *path_str;

	path_array = NULL;
	while (path_array == NULL && env_varibles[i] != NULL)
	{
		if (!ft_strncmp("PATH=", env_varibles[i], 5))
			path_array = ft_split(env_varibles[i] + 5, ':');
		i++;
	}
	return (path_array);
}

int ft_check_if_infile_exist_and_access(char *filename)
{
	if (access(filename, F_OK) == -1)
		return (errno);
	if (access(filename, R_OK) == -1)
		return (errno);
	return (0);
}

int ft_check_outfile_access(char *filename)
{
	if (access(filename, W_OK) == -1)
		return (errno);
	return (0);
}

void ft_free_struct(t_inputs *inputs)
{
	if (inputs)
	{
		if (inputs->first_cmd_with_path)
			free(inputs->first_cmd_with_path);
		if (inputs->last_cmd_with_path)
			free(inputs->last_cmd_with_path);
		free(inputs->fd_pipe);
		ft_free_splited_arrays(inputs->first_cmd);
		ft_free_splited_arrays(inputs->last_cmd);
		ft_free_splited_arrays(inputs->path);
		free(inputs);
	}
}

void ft_exit_on_err(char *message, int code, t_inputs *inputs)
{
	fprintf(stderr, "%s : %s\n", message, strerror(errno));
	perror("EXIT ON ERROR LEAKS ?");
}

int ft_is_cmds_executables(t_inputs *input_set)
{
	int is_cmd_valid;

	is_cmd_valid = 1;
	input_set->first_cmd_with_path =
	    create_cmd_with_path(input_set->path, input_set->first_cmd[0]);
	if (input_set->first_cmd_with_path)
		is_cmd_valid = 0;
	input_set->last_cmd_with_path =
	    create_cmd_with_path(input_set->path, input_set->last_cmd[0]);
	if (input_set->last_cmd_with_path)
		is_cmd_valid = 0;
	return (is_cmd_valid);
}

t_inputs *init_input_set(int len, char **input, char **envp)
{
	t_inputs *input_set;
	char     *complete_filename;

	input_set = malloc(sizeof(t_inputs));
	if (!input_set)
		return (NULL);
	input_set->input_array = input;
	input_set->path = ft_get_path_array(envp);
	complete_filename = create_filename_with_path(input_set->path, input[1]);
	input_set->first_cmd = ft_input_to_shell_format(input[2], complete_filename);
	input_set->last_cmd = ft_input_to_shell_format(input[len - 2], NULL);
	input_set->fd_pipe = ft_create_pipe();
	input_set->envp = envp;
	input_set->first_cmd_with_path = NULL;
	input_set->last_cmd_with_path = NULL;
	input_set->len = len;
	if (ft_is_cmds_executables(input_set))
		return (NULL);
	return (input_set);
}

void ft_child_process(t_inputs *input)
{
	if (ft_check_if_infile_exist_and_access(input->first_cmd[0]))
		return;
	dup2(input->fd_pipe[1], STDOUT_FILENO);
	ft_close_fds(input);
	if (execve(input->first_cmd_with_path, input->first_cmd, NULL) == -1)
		input->there_is_error = 1;
}

void ft_parent_process(t_inputs *input)
{
	int file_fd;

	if (ft_check_outfile_access(input->last_cmd[0]))
		return;
	file_fd = open(input->input_array[4], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (file_fd == -1)
		return;
	dup2(file_fd, STDOUT_FILENO);
	dup2(input->fd_pipe[0], STDIN_FILENO);
	ft_close_fds(input);
	if (execve(input->last_cmd_with_path, input->last_cmd, NULL) == -1)
		input->there_is_error = 1;
}

int main(int argc, char **argv, char **envp)
{
	int       pid1;
	t_inputs *input_set;
	errno = 0;

	check_valid_nb_of_args(argc);
	input_set = init_input_set(argc, argv, envp);
	if (!input_set)
		return (errno);
	pid1 = fork();
	// double fork or waitpid before freeing
	if (pid1 != -1)
	{
		if (pid1 == 0)
			ft_child_process(input_set);
		if (pid1 != 0)
		{
			wait(NULL);
			if (input_set->there_is_error)
				return errno;
			ft_parent_process(input_set);
		}
	}
	ft_free_struct(input_set);
	return (errno);
}

// 127 commande introuvable
