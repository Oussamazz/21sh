/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_blt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 17:25:44 by macos             #+#    #+#             */
/*   Updated: 2020/12/21 21:01:43 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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
        else if ((bin_path = get_bin_file(cmd + i, env_list)))
        {
            ft_putstr_fd(cmd[i], 1);
            ft_putstr_fd(" is ", 1);
            ft_putendl_fd(bin_path, 1);
            ft_strdel(&bin_path);
        }
        else
            ft_putendl_fd_error("21sh: type: ", cmd[i], ": not found.\n", NULL);
        i++;
    }
    return ;
}
