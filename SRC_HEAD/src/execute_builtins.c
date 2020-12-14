/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 17:40:42 by macos             #+#    #+#             */
/*   Updated: 2020/12/14 20:02:06 by oelazzou         ###   ########.fr       */
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


static void cd_simple(char *path, t_env **env_list)
{
    struct stat st;
    char *tmp;
    char *tmp2;

    if (path && access(path, F_OK) == 0)
    {
        stat(path, &st);
        if (!S_ISDIR(st.st_mode))
            ft_putendl_fd_error("21sh: cd: not a directory: ", ft_strrchr(path, '/') + 1, "\n", NULL);
        if (access(path, X_OK) == 0)
		{
			gen_oldpwd((tmp = get_cwd()), env_list); // this one
			chdir(path);
			gen_pwd((tmp2 = get_cwd()), env_list); // and this one
			ft_strdel(&tmp);
			ft_strdel(&tmp2);
		}
		else
			ft_putendl_fd_error("21sh: cd: permission denied: ", ft_strrchr(path, '/') + 1, "\n", NULL);
    }
    else
        ft_putendl_fd_error("21sh: cd: no such file or directory: ", ft_strrchr(path, '/') + 1, "\n", NULL);
}

static void cd_home(char *path, t_env **env_list)
{
    char *home;

    home = NULL;
    if (!(home = get_value_expansion("HOME", env_list)))
        return ;
    cd_simple(home, env_list);
    ft_strdel(&home);
}

static void cd_back(t_env **env_list)
{
    char *back_path;

    back_path = NULL;
    if (!(back_path = get_value_expansion("OLDPWD", env_list)))
        return ;
    cd_simple(back_path, env_list);
    ft_strdel(&back_path);
}

void    blt_cd(char **cmd, t_env **env_list)
{
    char    *new_path;
    char    *cwd;
    char    buff[MAX_INDEX];
                                                // cd /Users  ; cd - ; cd cd .. ; cd ;
    new_path = NULL;
    cwd = NULL;
    if (check_args_no(cmd) > 2)
        return (ft_putendl_fd("21sh: cd: Too many arguments.", 2));
    if (check_args_no(cmd) == 1)
        return (cd_home(cmd[0], env_list));
    if (check_args_no(cmd) == 2)
    {
        ft_strcpy(buff, cmd[1]);
        if (buff[0] != '/' && buff[0] != '.' && buff[0] != '-')
        {
            if (!(cwd = get_cwd()))
                return ;
            ft_strcat(cwd, "/");
            if (!(new_path = ft_strjoin(cwd, buff)))
                return ;
            cd_simple(new_path, env_list);
            ft_strdel(&new_path);
            ft_strdel(&cwd);
            return ;            
        }
        if (cmd[1][0] == '-' && cmd[1][1] == '\0')
		    cd_back(env_list);
        else
            cd_simple(buff, env_list); // cd /bin/ls
    }                                 
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
    if ((check_args_no(cmd)) == 1)
        return (print_env_list(env_list));
    if ((check_args_no(cmd)) != 3)
        return (ft_putendl_fd("21sh: Error: [setenv [var_name] [var_value]].", 2));
    else if (!ft_isalpha(cmd[1][0]))
        return (ft_putendl_fd("21sh: setenv: Variable name must begin with a letter.", 2));
    if (env_exist(env_list, cmd[1]) == 0)
        addtolist(env_list, ft_strdup(cmd[1]), ft_strdup(cmd[2]));
    else
        modify_env(env_list, cmd[1], cmd[2]);
    return ;
}

void    execute_builtin(char **cmd, char **tabs, t_env **env_list) // <- t_miniast **
{
    if (cmd && tabs && *env_list)
    {
        if (ft_strequ(cmd[0] , "echo"))
            blt_echo(cmd);
        else if (ft_strequ(cmd[0] , "cd"))
            blt_cd(cmd, env_list);
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