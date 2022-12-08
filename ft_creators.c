/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_creators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:43:10 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/27 00:58:52 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"
#include <stdio.h>

int	*ft_create_pipe(void)
{
	int	*fd;

	fd = malloc(2 * sizeof(int));
	if (!fd)
		return (NULL);
	if (pipe(fd) == -1)
	{
		perror("Pipe creation error");
		exit(127);
	}
	return (fd);
}

char	**ft_get_path_array(char **env_varibles)
{
	int		i;
	char	**path_array;

	i = 0;
	path_array = NULL;
	while (env_varibles[i] != NULL)
	{
		if (!ft_strncmp("PATH=", env_varibles[i], 5))
		{
			path_array = ft_split(env_varibles[i] + 5, ':');
			break ;
		}
		i++;
	}
	return (path_array);
}

char	*create_cmd_with_path(char **path, char *cmd)
{
	int		i;
	int		len;
	char	*cmd_with_path;

	if (!access(cmd, X_OK))
	{
		cmd_with_path = strdup(cmd);
		return (cmd_with_path);
	}
	i = 0;
	while (path[i])
	{
		len = ft_strlen(cmd) + ft_strlen(path[i]) + 2;
		cmd_with_path = ft_calloc(len, sizeof(char));
		if (cmd_with_path == NULL)
			break ;
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

char	*create_file_with_path(char **path, char *filename)
{
	int		i;
	int		len;
	char	*file_with_path;

	i = 0;
	if (!access(filename, R_OK))
		return (ft_strdup(filename));
	while (path[i])
	{
		len = ft_strlen(filename) + ft_strlen(path[i]) + 2;
		file_with_path = ft_calloc(len, sizeof(char));
		if (file_with_path == NULL)
			break ;
		ft_strlcat(file_with_path, path[i], len);
		ft_strlcat(file_with_path, "/", len);
		ft_strlcat(file_with_path, filename, len);
		if (!access(file_with_path, X_OK))
			return (file_with_path);
		free(file_with_path);
		i++;
	}
	return (NULL);
}

void	init_set(t_inputs *set, int len, char **argc, char **added_env_var)
{
	extern char	**environ;

	set->len = len;
	set->input = argc;
	set->fd_pipe = ft_create_pipe();
	set->path = ft_get_path_array(environ);
	set->cmd_name = NULL;
	set->separator = ' ';
	set->err_code = 0;
	set->added_env_var = added_env_var;
}
