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

#include "libft/libft.h"
#include "pipex.h"
#include <stddef.h>
#include <stdio.h>

void	ft_error_message(char *filename, int error)
{
	write(2, "pipex: line 1:  ", 15);
	write(2, filename, ft_strlen(filename));
	if (error == NO_FILE)
		write(2, ": No such file or directory\n", 28);
	else if (error == CMD_NOT_FOUND)
		write(2, ": command not found\n", 20);
	else if (error == PERM)
		write(2, ": No such file or directory\n", 28);
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

char	handle_if_quoted(char **cmd_str)
{
	int		i;
	int		nb_of_quotes;
	char	character;

	i = 0;
	nb_of_quotes = 0;
	character = 0;
	if (!ft_strchr(*cmd_str, '\'') && !ft_strchr(*cmd_str, '\"'))
		return (' ');
	while ((*cmd_str)[i] != '\'' && (*cmd_str)[i] != '\"')
		i++;
	character = (*cmd_str)[i];
	i = 0;
	while ((*cmd_str)[i])
	{
		if ((*cmd_str)[i] == character)
			nb_of_quotes++;
		if (!(nb_of_quotes % 2) && (*cmd_str)[i] == ' ')
			(*cmd_str)[i] = character;
		i++;
	}
	return (character);
}

void	ft_free_struct(t_inputs *set)
{
	free(set->fd_pipe);
	ft_free_splited_arrays(set->path);
}
