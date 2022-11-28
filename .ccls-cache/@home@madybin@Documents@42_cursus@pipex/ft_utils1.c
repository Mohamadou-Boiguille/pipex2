#include "pipex.h"

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

void ft_close_fds(t_inputs *input_set)
{
	close(input_set->fd_pipe[0]);
	close(input_set->fd_pipe[1]);
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
