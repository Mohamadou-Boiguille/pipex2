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

int	path_doesnt_exist(char *filename)
{
	char	*path_only;
	char	*last_slash;
	int		path_doesnt_exist;

	path_doesnt_exist = 1;
	last_slash = ft_strrchr(filename, 47);
	if (last_slash == NULL)
		return (0);
	path_only = ft_calloc(last_slash - filename + 2, sizeof(char));
	if (!path_only)
	{
		perror("malloc error (if_path_doesnt_exist)\n");
		return (0);
	}
	ft_strlcpy(path_only, filename, last_slash - filename + 1);
	if (!access(path_only, F_OK))
		path_doesnt_exist = 0;
	free(path_only);
	if (path_doesnt_exist)
		ft_error_message(filename, NO_FILE);
	return (path_doesnt_exist);
}

int	cmd_is_executable(char *cmd_with_path)
{
	if (cmd_with_path == NULL)
		return (0);
	return (!access(cmd_with_path, X_OK));
}

int	no_have_file_permission(char *filename, int type)
{
	if (type == READ && access(filename, R_OK))
	{
		ft_error_message(filename, PERM);
		return (1);
	}
	if (type == WRITE && !access(filename, F_OK) && access(filename, W_OK))
	{
		ft_error_message(filename, PERM);
		return (1);
	}
	return (0);
}

int	no_infile_or_no_access(t_inputs *set)
{
	int	file_access;

	set->filename = create_file_with_path(set->path, set->input[1]);
	if (set->filename == NULL)
		return (errno);
	else
		file_access = access(set->filename, F_OK && R_OK);
	if (file_access)
		return (errno);
	return (file_access);
}
