/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checkers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:38:05 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/24 22:38:46 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

int	ft_is_cmds_executables(t_inputs *input_set)
{
	input_set->first_cmd_with_path =
		create_cmd_with_path(input_set->path, input_set->first_cmd[0]);
	if (!input_set->first_cmd_with_path)
		return (0);
	input_set->last_cmd_with_path =
		create_cmd_with_path(input_set->path, input_set->last_cmd[0]);
	if (!input_set->last_cmd_with_path)
    {
        free(input_set->first_cmd_with_path);
		return (0);
    }
	return (1);
}

void	check_valid_nb_of_args(int nb_of_args)
{
	if (nb_of_args < 5)
	{
		fprintf(stderr, "Mauvais nombre d'arguments : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int	no_infile_or_no_access(t_inputs *set)
{
    int file_access;

    set->in_filename = create_file_with_path(set->path, set->input[1]);
    if (set->in_filename == NULL)
        return (errno);
    else
        file_access = access(set->in_filename, F_OK && R_OK);
    if (file_access)
    {
        return (errno);
    }
	return (file_access);
}
