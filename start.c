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

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_inputs	input_set;
	int			error_code;

	if (argc != 5)
	{
		perror("Wrong number of arguments");
		exit(EXIT_FAILURE);
	}
	ft_init_set(&input_set, argc, argv, envp);
	error_code = ft_fork_and_pipe(&input_set);
	free(input_set.fd_pipe);
	ft_free_splited_arrays(input_set.path);
	return (error_code);
}

int	ft_fork_and_pipe(t_inputs *set)
{
	int	pid1;
	int	pid2;
	int	pid_status;

	pid2 = 2022;
	pid1 = fork();
	if (pid1 == -1)
		return (-1);
	if (pid1 == 0)
		ft_infile_process(set);
	pid2 = fork();
	if (pid2 == -1)
		return (-1);
	if (pid2 == 0)
		ft_outfile_process(set);
	close(set->fd_pipe[0]);
	close(set->fd_pipe[1]);
	waitpid(pid1, &pid_status, 0);
	waitpid(pid2, &pid_status, 0);
	return (WEXITSTATUS(pid_status));
}

void	ft_infile_process(t_inputs *set)
{
	if (ft_path_doesnt_exist(set->input[1]))
		return ;
	if (ft_no_have_file_permission(set->input[1], READ))
		return ;
	set->in_fd = open(set->input[1], O_RDONLY);
	if (set->in_fd == -1)
		ft_error_message(set->input[1], NO_FILE);
	else if (dup2(set->in_fd, STDIN_FILENO) <= -1)
		return (perror("dup failed infile"));
	else if (dup2(set->fd_pipe[1], STDOUT_FILENO) <= -1)
		return (perror("dup failed pipe"));
	else
	{
		close(set->fd_pipe[0]);
		close(set->fd_pipe[1]);
        close(set->in_fd);
		ft_execute_cmd(set->input[2], set);
	}
	free(set->fd_pipe);
	ft_free_splited_arrays(set->path);
	exit(127);
}

void	ft_outfile_process(t_inputs *set)
{
	if (ft_path_doesnt_exist(set->input[set->len - 1]))
		return ;
	if (ft_no_have_file_permission(set->input[set->len - 1], WRITE))
		return ;
	set->out_fd = open(set->input[set->len - 1], CREAT | TRUNC | RDWR, RWALL);
	if (set->out_fd == -1)
		ft_error_message(set->input[set->len - 1], NO_FILE);
	else if (dup2(set->fd_pipe[0], STDIN_FILENO) == -1)
		return (perror("dup failed pipe2"));
	else if (dup2(set->out_fd, STDOUT_FILENO) == -1)
		return (perror("dup failed outfile"));
	else
	{
		close(set->fd_pipe[0]);
		close(set->fd_pipe[1]);
        close(set->out_fd);
		ft_execute_cmd(set->input[set->len - 2], set);
	}
	free(set->fd_pipe);
	ft_free_splited_arrays(set->path);
	exit(127);
}

void	ft_execute_cmd(char *cmd, t_inputs *set)
{
	set->separator = ft_handle_if_quoted(&cmd);
	set->cmd_with_args = ft_split(cmd, set->separator);
	set->cmd_name = ft_create_cmd_with_path(set->path, set->cmd_with_args[0]);
	if (set->cmd_name)
		execve(set->cmd_name, set->cmd_with_args, set->added_env_var);
	free(set->cmd_name);
	ft_free_splited_arrays(set->cmd_with_args);
	ft_free_splited_arrays(set->path);
	free(set->fd_pipe);
	ft_error_message(cmd, CMD_NOT_FOUND);
	exit(127);
}
