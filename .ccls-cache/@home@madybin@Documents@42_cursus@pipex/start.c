#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include "pipex.h"

int		length_of_first_command;

char **set_env_variables(void)
{
  char **envp = malloc(5 * sizeof(char *));
  envp[0] = "PATH=/bin:/usr/bin";
  envp[1] = NULL;
  return (envp);
}

void	ft_free_array(char **str_arr)
{
	int	i;

	i = 0;
	if (!str_arr)
		return ;
	while (str_arr[i] != NULL)
		free(str_arr[i++]);
	free(str_arr);
}

int	not_enough_args(int nb_of_args)
{
	if (nb_of_args < 5)
  {
    perror("input is less than 4");
		return (0);
  }
	return (1);
}

char	*create_cmd_str_with_path(char *cmd)
{
	char	*cmd_str_with_path;

	cmd_str_with_path = malloc(10 + ft_strlen(cmd));
	ft_strlcpy(cmd_str_with_path, "/usr/bin/", 10);
	ft_strlcat(cmd_str_with_path, cmd, 10 + ft_strlen(cmd));
	return (cmd_str_with_path);
}

int	ft_execute_first_cmd(char *cmd_input_format, char *file)
{
	char	**cmd_and_args;
	char	*cmd_str_with_path;

	cmd_and_args = ft_split_and_add_file(cmd_input_format, ' ', file);
	//cmd_str_with_path = create_cmd_str_with_path(cmd_and_args[0]);
	if (execve(cmd_and_args[0], cmd_and_args, NULL) == -1)
  {
    perror("execution error");
    return (-1);
  }
	ft_free_array(cmd_and_args);
	free(cmd_str_with_path);
	return (1);
}

int	ft_execute_last_cmd(char *cmd_input_format)
{
	int		i;
	char	**cmd_and_args;
	char	*cmd_str_with_path;

	i = 0;
	cmd_and_args = ft_split_and_add_file(cmd_input_format, ' ', NULL);
	cmd_str_with_path = create_cmd_str_with_path(cmd_and_args[0]);
	i = execve(cmd_str_with_path, cmd_and_args, NULL);
	ft_free_array(cmd_and_args);
	free(cmd_str_with_path);
	return (1);
}

int	main(int argc, char **argv)
{
	int	pid1;
	int	pid2;
	int	fd[2];
	int	fd_memory;
	int	retfd;

	if (!not_enough_args(argc))
		return (127);
	if (pipe(fd) == -1)
	{
		perror("pipe error");
		return (-1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork error");
		return (-1);
	}
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (ft_execute_first_cmd(argv[2], argv[1]) == -1)
      return (127);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork error");
		return (-1);
	}
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		retfd = open(argv[4], O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG);
		if (retfd == -1)
    {
      perror("open file error");
			return (0);
    }
		dup2(retfd, STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		ft_execute_last_cmd(argv[3]);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
