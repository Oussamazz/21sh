/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/15 15:30:12 by macos             #+#    #+#             */
/*   Updated: 2020/11/07 23:16:50 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

static int addtolist(t_env **head, char *var_name, char *var_value)
{
    t_env *tmp;
    t_env *new;

    if (!(new = (t_env*)ft_memalloc(sizeof(t_env))))
        return (-1);
    new->env_var_name = var_name;
    new->env_var_value = var_value;
    new->next = NULL;
    if (!(*head))
        *head = new;
    else
    {
        tmp = *head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    return (0);   
}

t_env      *stock_env(char **env)
{
    t_env *list;
    char    *var_value;
    char    *var_name;
    int i;
    size_t len;

    list = NULL;
    if (!env || !*env)
        return (NULL);
    i = 0;
    while (env[i] != NULL)
    {
        len = ft_strlen_char(env[i], '=');
        var_name = ft_strsub(env[i], 0, len);
        var_value = ft_strsub(env[i],len + 1, ft_strlen(env[i]) - len - 1);
        addtolist(&list, var_name, var_value);
        i++;
    }
    return (list);
}
