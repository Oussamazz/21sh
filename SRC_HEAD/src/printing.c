/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:19:31 by macos             #+#    #+#             */
/*   Updated: 2020/12/12 02:30:00 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

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

void    starting_message(char *argv, char **user, time_t *now)
{
    if (ft_strequ(argv, "-s") && *user)
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
    }
    else
        error_message("21sh: Error: Flag not found: try[[./21sh] -s] or [./21sh]\n", 1);
}
