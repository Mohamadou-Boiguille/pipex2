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

int	ft_is_cmds_executables(t_inputs *input_set)
{
	int	is_cmd_valid;

	input_set->first_cmd_with_path =
		create_cmd_with_path(input_set->path, input_set->first_cmd[0]);
	if (!input_set->first_cmd_with_path)
		return (0);
	input_set->last_cmd_with_path =
		create_cmd_with_path(input_set->path, input_set->last_cmd[0]);
	if (!input_set->last_cmd_with_path)
		return (0);
	perror("valid cmd");
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

int	ft_check_if_infile_exist_and_access(char *filename)
{
	if (access(filename, F_OK) == -1)
		return (errno);
	if (access(filename, R_OK) == -1)
		return (errno);
	return (0);
}

int	ft_check_outfile_access(char *filename)
{
	if (access(filename, W_OK) == -1)
		return (errno);
	return (0);
}
