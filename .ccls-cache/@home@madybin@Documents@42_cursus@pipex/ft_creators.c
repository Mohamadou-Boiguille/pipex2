/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_creators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:43:10 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/24 22:45:37 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	*ft_create_pipe(void)
{
	int	*fd;

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

char	**ft_input_to_shell_format(char *cmd_and_args, char *filename)
{
	int		cmd_array_size;
	char	*shell_format_cmd;
	char	**cmd_array;
	int		str_size;

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

char	**ft_get_path_array(char **env_varibles)
{
	int		i;
	char	**path_array;
	char	*path_str;

	i = 0;
	path_array = NULL;
	while (path_array == NULL && env_varibles[i] != NULL)
	{
		if (!ft_strncmp("PATH=", env_varibles[i], 5))
			path_array = ft_split(env_varibles[i] + 5, ':');
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
	i = -1;
	while (path[++i])
	{
		len = ft_strlen(cmd) + ft_strlen(path[i]) + 2;
		cmd_with_path = ft_calloc(len, sizeof(char));
		if (cmd_with_path == NULL)
			break ;
		ft_strlcat(cmd_with_path, path[i], len);
		ft_strlcat(cmd_with_path, "/", len);
		ft_strlcat(cmd_with_path, cmd, len);
		if (!access(cmd_with_path, X_OK))
		{
			errno = 0;
			return (cmd_with_path);
		}
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

	i = -1;
	if (!access(filename, R_OK))
		return (filename);
	while (path[++i])
	{
		len = ft_strlen(filename) + ft_strlen(path[i]) + 2;
		file_with_path = ft_calloc(len, sizeof(char));
		if (file_with_path == NULL)
			break ;
		ft_strlcat(file_with_path, path[i], len);
		ft_strlcat(file_with_path, "/", len);
		ft_strlcat(file_with_path, filename, len);
		if (!access(file_with_path, X_OK))
		{
			errno = 0;
			return (file_with_path);
		}
		free(file_with_path);
	}
	return (NULL);
}
