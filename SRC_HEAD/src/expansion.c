/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:24:04 by macos             #+#    #+#             */
/*   Updated: 2020/11/21 12:21:01 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static size_t get_size_expansion(char *exp)
{
    int i;
    int len;

    i = 0;
    len = 0;
    if ((exp[0] == '$' && exp[1] != '\0') || (exp[0] == '~'))
        i++;
    while (exp[i] != '\0')
    {
        if (ft_isalnum(exp[i]) || exp[i] == 47)
            len++;
        else if (is_blank(exp[i]))
            break ;
        i++;
    }
    if (exp[0] == '~' && !exp[1])
        return (1);
    return (len);
}

int         ft_is_expansion(char *str)
{
    if (str && *str)
    {
        if (str[0] == '$' && ft_isalnum(str[1]) && !is_quote(str[1]))
            return (1);
    }
    return (0);
}

int     ft_is_tilde(char *str)
{
    if (str && *str)
    {
        if (str[0] == '~' && ft_isascii(str[1]) && !is_quote(str[1]) || (str[0] == '~' && str[1] == 47))
            return (1);
    }
    return (0);
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
                return ((env_value = ft_strdup(curr->env_var_value)));
            curr = curr->next;
        }
    }
    return (env_value);
}

static void tilde_exp(char *exp, char **data, t_env **env_list)
{
    int i;
    char *home_value;
    char *user;
    char *user_name;

    if (exp && *exp)
    {
        i = 0;
        home_value = NULL;
        user = NULL;
        user_name = NULL;
        if (exp[i] == '~')
        {
            home_value = get_value_expansion("HOME", env_list);
            if (home_value == NULL)
            {
                user = get_value_expansion("USER", env_list);
                if (!(home_value = ft_strjoin("/Users/", user)))
                    error_message("HOME and USER are unset\n", 1);
                ft_strdel(&user);
            }
            if (exp[i + 1] == 47 && ft_isalpha(exp[i + 2]))
            {
                if(!(user_name = ft_strsub(exp + 1, 0, ft_strlen(exp + 1))))
                    return ;
                if (user_name)
                {
                    *data = ft_strjoin(home_value, user_name);
                    printf("user_name = %s || home_value = %s\ndata = %s\n", user_name, home_value, *data);
                    ft_strdel(&home_value);
                    ft_strdel(&user_name);
                    return ;
                }
            }
            *data = ft_strdup(home_value);
            ft_strdel(&home_value);
            ft_strdel(&user_name);
        }
    }
    return ;
}

int     expansion_parse(t_lexer **token_node, char *buf, t_env **env_list, t_pointt *cor)
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
        ft_putstr_fd("data size = ", 1);
        ft_putnbr_fd(data_size, 1);
        ft_putchar_fd('\n', 1);
        if (data_size > 0)
        {
            if (!(data = ft_strnew(data_size)))
                return (0);
            if (buf[i] && (buf[i] == '$'))
                i++;
            j = 0;
            while (buf[i] && (ft_isalnum(buf[i]) || buf[i] == '~') && i <= data_size)
            {
                if (i == 0 && ft_is_tilde(buf + i))
                {
                    tilde_exp(buf + i, &env_value, env_list);
                    if (env_value)
                        append_list(token_node, env_value, EXPANSION, cor);
                    ft_strdel(&env_value);
                    return (data_size);
                }
                data[j++] = buf[i++];
            }
            env_value = get_value_expansion(data, env_list);
            if (env_value)
                append_list(token_node, env_value, EXPANSION, cor);
            ft_strdel(&env_value);
        }
    }
    return (data_size);
}

