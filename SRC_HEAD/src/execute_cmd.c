/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 18:10:21 by macos             #+#    #+#             */
/*   Updated: 2020/12/12 21:32:52 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int check_builtins(char *cmd_name)
{
    if (ft_strequ(cmd_name, "echo") || ft_strequ(cmd_name, "setenv") ||
     ft_strequ(cmd_name, "unsetenv") ||
        ft_strequ(cmd_name, "env") || ft_strequ(cmd_name, "cd") ||
         ft_strequ(cmd_name, "exit"))
         return (1);
    return (0);
        
}

void	execute_direct(char **cmd, char **tabs)
{
    const char *file_name;
	pid_t	pid;

    if (cmd[0][0] == '.' && cmd[0][1] == '/')
        file_name = ft_strrchr(cmd[0], '/') + 1;
    else
        file_name = cmd[0];
	if ((pid = fork()) < 0)
        return (ft_putendl_fd("21sh: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(file_name, F_OK) == 0)
		{
			if (access(file_name, X_OK) == 0)
			{
				if (execve(file_name, cmd, tabs) == -1)
                {
					ft_putendl_fd("21sh: Error: Execution Failed.", 2);
                    exit (1);
                }
                exit(0);
			}
            else
                ft_putendl_fd("21sh: Error: permission denied.", 2);
		}
        else
            ft_putendl_fd("21sh: no such file or directory:", 2);
		exit(1);
	}
	else
		wait(NULL);
    return ;
}

void	execute_undirect(char **cmd, char **tabs, t_env **env)
{
    char *bin_file;
	pid_t	pid;

    if (check_builtins(cmd[0]))
        return (execute_builtin(cmd, tabs, env));
    else if (!(bin_file = get_bin_file(cmd, env)))
        return (ft_putendl_fd("21sh: Error: command not found.", 2));
	if ((pid = fork()) < 0)
        return (ft_putendl_fd("21sh: Error: forking Failded.", 2));
	if (pid == 0)
	{
		if (access(bin_file, F_OK) == 0)
		{
			if (access(bin_file, X_OK) == 0)
			{
				if (execve(bin_file, cmd, tabs) == -1)
                {
					ft_putendl_fd("21sh: Error: Execution Failed.", 2);
                    exit (1);
                }
                exit(0);
			}
            else
                ft_putendl_fd("21sh: Error: permission denied.", 2);
		}
        else
            ft_putendl_fd("21sh: Error: command not found.", 2);
		exit(1);
	}
	else
		wait(NULL);
    ft_strdel(&bin_file);
    return ;
}

char        *get_bin_file(char **cmd,  t_env **env) //       cmd =  ls  -la     ||        /bin  /sbin  /usr/bin .....  
{
    int i;
    char *bin_file;
    char *env_path_value;
    char **dirs;

    bin_file = NULL;
    env_path_value = NULL;
    if (cmd[0] && env)
    {
        if (!(env_path_value = get_value_expansion("PATH", env)))
        {
            ft_putendl_fd("21sh: Error: PATH variable not found.", 2);
            return (NULL);
        }
        if (!(dirs = ft_strsplit(env_path_value, ':')))
            return (NULL);
        ft_strdel(&env_path_value);
        i = -1;
        while (dirs[++i] != NULL)
        {
            if (!(bin_file = ft_strjoin(ft_strcat(dirs[i], "/"), cmd[0])))
                return (NULL);
            if (access(bin_file, F_OK) == 0)
                break ;
            ft_strdel(&bin_file);
        }
        ft_free_arr(dirs);
        return (bin_file);
    }
    return (bin_file);
}