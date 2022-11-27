/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:34:28 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/26 23:30:26 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stddef.h>

void	ft_error_message(char *filename, int error)
{
		write(2, "pipex: line 1:  ", 15);
		write(2, filename, ft_strlen(filename));
		if (error == NO_FILE)
			write(2, ": No such file or directory\n", 28);
		if (error == CMD_NOT_FOUND)
			write(2, ": command not found\n", 20);
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

void	ft_close_fds(t_inputs *input_set)
{
	close(input_set->fd_pipe[0]);
	close(input_set->fd_pipe[1]);
}

int	ft_find_nb_of_words(char *str)
{
	size_t	i;
	int		nb_of_words;

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
