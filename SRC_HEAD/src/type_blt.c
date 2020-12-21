/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_blt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 17:25:44 by macos             #+#    #+#             */
/*   Updated: 2020/12/21 20:50:33 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char        *get_bin_file2(char **cmd,  t_env **env) //       cmd =  ls  -la     ||        /bin  /sbin  /usr/bin .....  
{
    int i;
    char *bin_file;
    char *env_path_value;
    char **dirs;

    bin_file = NULL;
    env_path_value = NULL;
    dirs = NULL;
    if (cmd[0] && env)
    {
        if (!(env_path_value = get_value_expansion("PATH", env)))
            return (NULL);
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

static void print_blt(char *cmd_name)
{
    ft_putstr_fd(cmd_name, 1);
    ft_putstr_fd(" is a shell builtin.\n", 1);
    return ;
}

void        type_builtin(char **cmd, t_env **env_list)
{
    int i;
    char *bin_path;

    bin_path = NULL;
    i = 1;
    while (cmd[i] && env_list)
    {
        if (check_builtins(cmd[i]))
            print_blt(cmd[i]);
        else if ((bin_path = get_bin_file2(cmd + i, env_list)))
        {
            ft_putstr_fd(cmd[i], 1);
            ft_putstr_fd(" is ", 1);
            ft_putendl_fd(bin_path, 1);
            ft_strdel(&bin_path);
        }
        else
            ft_putendl_fd_error("21sh: type: ", cmd[i], ": not found\n", NULL);
        i++;
    }
    return ;
}
