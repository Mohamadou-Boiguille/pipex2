/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboigui <moboigui@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 22:17:18 by moboigui          #+#    #+#             */
/*   Updated: 2022/11/26 23:32:16 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define CMD_NOT_FOUND 1
# define NO_FILE 2
# define PERM 3
# define TWO_CHARS 2
# define CREAT O_CREAT
# define TRUNC O_TRUNC
# define RDWR O_RDWR
# define RWALL 00666 //read and write for all
# define READ 1
# define WRITE 2

typedef struct s_inputs
{
	char	**cmd_with_args;
	char	**input;
	char	**path;
	char	**envp;
	char	**added_env_var;
	char	*cmd_name;
	char	*filename;
	char	separator;
	int		in_fd;
	int		out_fd;
	int		*fd_pipe;
	int		len;
	int		err_code;
}			t_inputs;

int			no_have_file_permission(char *filename, int type);
int			path_doesnt_exist(char *filename);
int			cmd_is_executable(char *cmd_with_path);
char		handle_if_quoted(char **cmd_str);
void		execute_cmd(char *cmd, t_inputs *set);
void		ft_error_message(char *filename, int error);
int			no_outfile_access(t_inputs *set);
int			ft_fork_and_pipe(t_inputs *set);
void		infile_process(t_inputs *set);
void		outfile_process(t_inputs *set);
int			*ft_create_pipe(void);
void		ft_free_splited_arrays(char **str_arr);
int			ft_find_nb_of_words(char *str);
char		**ft_input_to_shell_format(char *cmd_and_args, char *filename);
char		**ft_get_path_array(char **env_varibles);
int			no_infile_or_no_access(t_inputs *set);
int			ft_check_outfile_access(char *filename);
void		ft_free_struct(t_inputs *set);
char		*create_cmd_with_path(char **path, char *cmd);
char		*create_file_with_path(char **path, char *filename);
int			ft_is_cmds_executables(t_inputs *input_set);
void		init_set(t_inputs *set, int len, char **argc, char **added_env_var);
void		ft_child_process(t_inputs *input);
void		ft_parent_process(t_inputs *input);
int			is_input_valid(int len);
#endif
