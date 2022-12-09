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

int	ft_path_doesnt_exist(char *filename)
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

int	ft_cmd_is_executable(char *cmd_with_path)
{
	if (cmd_with_path == NULL)
		return (0);
	return (!access(cmd_with_path, X_OK));
}

int	ft_no_have_file_permission(char *filename, int type)
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
