/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 22:41:18 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/24 22:42:25 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int			pid1;
	t_inputs	*input_set;

	check_valid_nb_of_args(argc);
	input_set = init_input_set(argc, argv, envp);
	if (!input_set)
		return (errno);
	pid1 = fork();
	errno = 0;
	if (pid1 != -1)
	{
		if (pid1 == 0)
			ft_child_process(input_set);
		if (pid1 != 0)
		{
			wait(NULL);
			if (input_set->there_is_error)
				return (errno);
			ft_parent_process(input_set);
		}
	}
	ft_free_struct(input_set);
	return (errno);
}

t_inputs	*init_input_set(int len, char **input, char **envp)
{
	t_inputs	*input_set;
	char		*filename;

	input_set = malloc(sizeof(t_inputs));
	if (!input_set)
		return (NULL);
	input_set->input_array = input;
	input_set->path = ft_get_path_array(envp);
	entire_filename = create_filename_with_path(input_set->path, input[1]);
	input_set->first_cmd = ft_input_to_shell_format(input[2], entire_filename);
	input_set->last_cmd = ft_input_to_shell_format(input[len - 2], NULL);
	input_set->fd_pipe = ft_create_pipe();
	input_set->envp = envp;
	input_set->first_cmd_with_path = NULL;
	input_set->last_cmd_with_path = NULL;
	input_set->len = len;
	if (!ft_is_cmds_executables(input_set))
	{
		ft_free_struct(input_set);
		perror("invalid command");
		return (NULL);
	}
	return (input_set);
}

void	ft_child_process(t_inputs *input)
{
	if (ft_check_if_infile_exist_and_access(input->first_cmd[0]))
		return ;
	dup2(input->fd_pipe[1], STDOUT_FILENO);
	ft_close_fds(input);
	if (execve(input->first_cmd_with_path, input->first_cmd, input->envp) == -1)
		input->there_is_error = 1;
}

void	ft_parent_process(t_inputs *input)
{
	int	file_fd;

	file_fd = open(input->last_cmd[0], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (file_fd == -1)
		return ;
	dup2(input->fd_pipe[0], STDIN_FILENO);
	dup2(file_fd, STDOUT_FILENO);
	ft_close_fds(input);
	if (execve(input->last_cmd_with_path, input->last_cmd, input->envp) == -1)
		input->there_is_error = errno;
	perror("execve error");
}
