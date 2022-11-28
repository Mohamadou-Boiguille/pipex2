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
#include <unistd.h>

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
	pid2 = fork();
	if (pid2 == -1)
		return (perror("fork error"));
	if (pid2 == 0)
		outfile_process(set);
	ft_close_fds(set);
	waitpid(pid1, &pid_status, 0);
	waitpid(pid2, &pid_status, 0);
}

void	infile_process(t_inputs *set)
{
	if (set->infile_fd == -1)
	{
		ft_error_message(set->input[1], NO_FILE);
		return ;
	}
	if (dup2(set->infile_fd, STDIN_FILENO) <= -1)
		return (perror("dup failed infile"));
	if (dup2(set->fd_pipe[1], STDOUT_FILENO) <= -1)
		return (perror("dup failed pipe"));
	close(set->infile_fd);
	ft_close_fds(set);
	execute_cmd(set->input[2], set);
}

void	outfile_process(t_inputs *set)
{
	set->outfile_fd = open(set->input[set->len - 1], O_CREAT | O_TRUNC | O_RDWR,
			00666);
	if (set->outfile_fd == -1)
	{
		ft_error_message(set->input[set->len - 1], NO_FILE);
		return ;
	}
	if (dup2(set->fd_pipe[0], STDIN_FILENO) == -1)
		return (perror("dup failed pipe2"));
	if (dup2(set->outfile_fd, STDOUT_FILENO) == -1)
		return (perror("dup failed outfile"));
	ft_close_fds(set);
	close(set->outfile_fd);
	execute_cmd(set->input[set->len - 2], set);
}

void	execute_cmd(char *cmd, t_inputs *set)
{
	set->separator = handle_if_quoted(&cmd);
	set->cmd_with_args = ft_split(cmd, set->separator);
	set->cmd_name = create_cmd_with_path(set->path, set->cmd_with_args[0]);
	execve(set->cmd_name, set->cmd_with_args, set->envp);
	ft_free_splited_arrays(set->cmd_with_args);
	free(set->cmd_name);
	ft_error_message(cmd, CMD_NOT_FOUND);
    exit(127);
}
