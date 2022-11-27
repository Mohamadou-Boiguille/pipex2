/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:41:18 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/26 23:36:14 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

int	main(int argc, char **argv, char **envp)
{
	t_inputs	input_set;

	errno = 0;
	check_valid_nb_of_args(argc);
	init_set(&input_set, argc, argv, envp);
	ft_fork_and_pipe(&input_set);
	free(input_set.fd_pipe);
	ft_free_splited_arrays(input_set.path);
	return (errno);
}

void	init_set(t_inputs *set, int len, char **argc, char **envp)
{
	set->len = len;
	set->envp = envp;
	set->input = argc;
	set->fd_pipe = ft_create_pipe();
	set->path = ft_get_path_array(envp);
	set->cmd_name = NULL;
}

void	ft_fork_and_pipe(t_inputs *set)
{
	int	pid1;
	int	pid2;
	int	pid_status;

	pid1 = fork();
	if (pid1 == -1)
		return (perror("fork error"));
	if (pid1 == 0)
		infile_process(set);
	if (!errno)
	{
		pid2 = fork();
		if (pid2 == -1)
			return (perror("fork error"));
		if (pid2 == 0)
			outfile_process(set);
	}
	ft_close_fds(set);
	if (!errno)
	{
		waitpid(pid2, &pid_status, 0);
		waitpid(pid1, &pid_status, 0);
	}
}

void	infile_process(t_inputs *set)
{
	int	infile_fd;

	infile_fd = open(set->input[1], O_RDONLY);
	if (infile_fd == -1)
	{
		ft_error_message(set->input[1], NO_FILE);
		return ;
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
		return (perror("dup failed infile"));
	if (dup2(set->fd_pipe[1], STDOUT_FILENO) == -1)
		return (perror("dup failed pipe"));
	ft_close_fds(set);
	set->cmd_with_args = ft_split(set->input[2], ' ');
	set->cmd_name = create_cmd_with_path(set->path, set->cmd_with_args[0]);
	execve(set->cmd_name, set->cmd_with_args, set->envp);
	ft_free_splited_arrays(set->cmd_with_args);
	free(set->cmd_name);
	ft_error_message(set->input[2], CMD_NOT_FOUND);
	errno = 127;
}

void	outfile_process(t_inputs *set)
{
	int	outfile_fd;

	outfile_fd = open(set->input[set->len - 1], O_CREAT | O_TRUNC | O_RDWR,
			00666);
	if (outfile_fd == -1)
	{
		ft_error_message(set->input[set->len - 1], NO_FILE);
		return ;
	}
	if (dup2(set->fd_pipe[0], STDIN_FILENO) == -1)
		return (perror("dup failed pipe2"));
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		return (perror("dup failed outfile"));
	ft_close_fds(set);
	set->cmd_with_args = ft_split(set->input[set->len - 2], ' ');
	set->cmd_name = create_cmd_with_path(set->path, set->cmd_with_args[0]);
	execve(set->cmd_name, set->cmd_with_args, set->envp);
	ft_free_splited_arrays(set->cmd_with_args);
	free(set->cmd_name);
	ft_error_message(set->input[set->len - 2], CMD_NOT_FOUND);
	errno = 127;
}
