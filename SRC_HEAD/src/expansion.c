/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:24:04 by macos             #+#    #+#             */
/*   Updated: 2020/11/13 23:31:22 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static size_t get_size_expansion(char *exp)
{
    int i;
    int len;

    i = 0;
    len = 0;
    if (exp[0] == '$' && exp[1] != '\0')
        i++;
    while (exp[i] != '\0')
    {
        if (ft_isalnum(exp[i]))
            len++;
        else if (is_blank(exp[i]))
            break ;
        i++;
    }
    return (len);
}

char        *get_value_expansion(char *env_name, t_env **head)
{
    char    *env_value;
    t_env   *curr;

    env_value = NULL;
    curr = *head;
    if (env_name && *(env_name) && head && curr)
    {
        while (curr)
        {
            if (ft_strequ(curr->env_var_name, env_name) == 1)
                return (ft_strdup(curr->env_var_value));
            curr = curr->next;
        }
    }
    return (env_value);
}

int     expansion_parse(t_lexer **token_node, char *buf, t_env **env_list)
{
    int     i;
    int     j;
    size_t  data_size;
    char    *data;
    char    *env_value;

    i = 0;
    env_value = NULL;
    data_size = 0;
    if (buf && *(buf + i))
    {
        data_size = get_size_expansion(buf + i);
        if (data_size > 0)
        {
            if (!(data = ft_strnew(data_size)))
                return (0);
            while (buf[i] && buf[i] == '$')
                i++;
            j = 0;
            while (buf[i] && ft_isalnum(buf[i]) && i <= data_size)
                data[j++] = buf[i++];
            env_value = get_value_expansion(data, env_list);
            if (env_value)
                append_list(token_node, env_value, EXPANSION);
            ft_strdel(&env_value);
        }
    }
    return (data_size);
}