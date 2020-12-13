/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 17:40:42 by macos             #+#    #+#             */
/*   Updated: 2020/12/13 00:53:53 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int check_args_no(char **cmd)
{
    int i;

    i = 0;
    while (cmd[i++] != NULL)
        ;
    return (i - 1);
}

void blt_unsetenv(char **cmd, t_env **env_list)
{
    int i;

    if (check_args_no(cmd) < 2)
        return (ft_putendl_fd("21sh: Error: [unsetenv [var_name] ...].", 2));
    i = 1;
    while (cmd[i])
    {
        deleteNode(env_list, cmd[i]);
        i++;
    }
    return ;
}

void    blt_setenv(char **cmd, t_env **env_list)
{
    if ((check_args_no(cmd)) != 3)
        return (ft_putendl_fd("21sh: Error: [setenv [var_name] [var_value]].", 2));
    else if (!ft_isalpha(cmd[1][0]))
        return (ft_putendl_fd("21sh: setenv: Variable name must begin with a letter.", 2));
    addtolist(env_list, ft_strdup(cmd[1]), ft_strdup(cmd[2]));
    return ;
}

void    execute_builtin(char **cmd, char **tabs, t_env **env_list) // <- t_miniast **
{
    if (cmd && tabs && *env_list)
    {
        if (ft_strequ(cmd[0] , "echo"))
            blt_echo(cmd);
        // else if (ft_strequ(cmd[0] , "cd"))
        //     blt_cd(cmd, tabs, env_list);
        else if (ft_strequ(cmd[0] , "setenv"))
            blt_setenv(cmd, env_list);
        else if (ft_strequ(cmd[0] , "unsetenv"))
            blt_unsetenv(cmd, env_list);
        else if (ft_strequ(cmd[0] , "env") && !cmd[1])
            print_env_list(env_list);
        // else if (ft_strequ(cmd[0] , "exit"))
        //     blt_exit(cmd, tabs, env_list);
    }
}