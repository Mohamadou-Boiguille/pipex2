/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checkers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:38:05 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/27 00:57:37 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

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
	int	file_access;

	set->in_filename = create_file_with_path(set->path, set->input[1]);
	if (set->in_filename == NULL)
		return (errno);
	else
		file_access = access(set->in_filename, F_OK && R_OK);
	if (file_access)
		return (errno);
	return (file_access);
}
