/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:19:31 by macos             #+#    #+#             */
/*   Updated: 2020/12/16 03:10:23 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    ft_putendl_fd_error(char *s1, char *s2, char *s3, char *s4)
{
    if (s1)
        ft_putstr_fd(s1, STDERR_FILENO);
    if (s2)
        ft_putstr_fd(s2, STDERR_FILENO);
    if (s3)
        ft_putstr_fd(s3, STDERR_FILENO);
    if (s4)
        ft_putstr_fd(s4, STDERR_FILENO);
    return ;
}

void    print_arr(char **arr)
{
    int i;
    if (arr)
    {
        i = 0;
        while (arr[i])
        {
            ft_putendl_fd(arr[i], 1);
            i++;
        }
    }
}

void    print_env_list(t_env **head)
{
    t_env *tmp;

    tmp = *head;
    while (tmp)
    {
        ft_putstr_fd(tmp->env_var_name, 1);
        ft_putstr_fd("=", 1);
        ft_putendl_fd(tmp->env_var_value, 1);
        tmp = tmp->next;
    }
}
static int chech_if(char **av)
{
    if (ft_is_there(av[1], 'g') || ft_is_there(av[2], 'G'))
        return (1);
    return (0);
}

void    starting_message(char **argv, char **user, time_t *now)
{
    if (chech_if(argv) && *user)
    {
        ft_putstr_fd("\e[1;1H\e[2J", 1);
        if (*user)
        {
            ft_putstr_fd(*user, 1);
            ft_strdel(user);
            ft_putstr_fd(" logged, ", 1);
            ft_putstr_fd(ttyname(0), 1);
        }
        ft_putstr_fd(WELCOME_MSG, 1);
        ft_putendl_fd(ctime(now), 1);
        g_graphic = 1;
    }
}
