/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:19:31 by macos             #+#    #+#             */
/*   Updated: 2020/11/13 22:20:40 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    print_env_list(t_env **head)
{
    t_env *tmp;

    tmp = *head;
    while (tmp)
    {
        ft_putstr(tmp->env_var_name);
        ft_putstr("=");
        ft_putendl(tmp->env_var_value);
        tmp = tmp->next;
    }
}
