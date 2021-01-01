/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 18:10:21 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 14:24:36 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int				check_builtins(char *cmd_name)
{
	if (ft_strequ(cmd_name, "echo") || ft_strequ(cmd_name, "setenv") ||
		ft_strequ(cmd_name, "unsetenv") || ft_strequ(cmd_name, "env") ||
		ft_strequ(cmd_name, "cd") || ft_strequ(cmd_name, "type") ||
			ft_strequ(cmd_name, "exit"))
		return (1);
	return (0);
}

void			my_execve(const char *file_name, char **cmd, char **tabs)
{
	if (access(file_name, X_OK) == 0)
	{
		if (execve(file_name, cmd, tabs) == -1)
		{
			ft_putendl_fd("21sh: Error: Execution Failed.", 2);
			exit(1);
		}
		exit(0);
	}
	else
		ft_putendl_fd_error("21sh: permission denied: ",
			cmd[0], "\n", NULL);
	return ;
}

void			execute_direct(char **cmd, char **tabs)
{
	const char	*file_name;
	pid_t		pid;

	if (cmd[0][0] == '.' && cmd[0][1] == '/')
		file_name = ft_strrchr(cmd[0], '/') + 1;
	else
		file_name = cmd[0];
	if ((pid = fork()) < 0)
		return (ft_putendl_fd("21sh: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(file_name, F_OK) == 0)
			my_execve(file_name, cmd, tabs);
		else
			ft_putendl_fd_error("21sh: no such file or directory: ",
				cmd[0], "\n", NULL);
		exit(1);
	}
	else
		wait(NULL);
}

void			execute_undirect(char **cmd, char **tabs, t_env **env)
{
	char		*bin_file;
	pid_t		pid;

	if (!(bin_file = get_bin_file(cmd, env)))
	{
		ft_putendl_fd_error("21sh: command not found: ", cmd[0], "\n", NULL);
		return ;
	}
	if ((pid = fork()) < 0)
		return (ft_putendl_fd("21sh: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(bin_file, F_OK) == 0)
			my_execve(bin_file, cmd, tabs);
		else
			ft_putendl_fd_error("21sh: no such file or directory: ",
				cmd[0], "\n", NULL);
		exit(1);
	}
	else
		wait(NULL);
	if (bin_file)
		ft_strdel(&bin_file);
}

char			*get_bin_file(char **cmd, t_env **env)
{
	t_get_bin	v;

	ft_bzero(&v, sizeof(t_get_bin));
	if (!(v.env_path_value = get_value_expansion("PATH", env)))
		return (NULL);
	if (!(v.dirs = ft_strsplit(v.env_path_value, ':')))
		return (NULL);
	ft_strdel(&v.env_path_value);
	v.i = -1;
	while (v.dirs[++v.i] != NULL)
	{
		if (!(v.tmp = ft_strdup(v.dirs[v.i])))
			return (NULL);
		v.tmp2 = ft_freejoin(v.tmp, "/", 0);
		if (!(v.bin_file = ft_strjoin(v.tmp2, cmd[0])))
			return (NULL);
		ft_strdel(&v.tmp2);
		if (access(v.bin_file, F_OK) == 0)
			break ;
		ft_strdel(&v.bin_file);
	}
	ft_free_arr(v.dirs);
	return (v.bin_file);
}
