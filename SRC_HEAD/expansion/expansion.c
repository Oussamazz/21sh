/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:24:04 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/26 14:44:45 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int get_size_expansion(char *exp)
{
    int i;
    int len;

    i = 0;
    len = 0;
    if (((exp[0] == '$' && !exp[1]) || (exp[0] == '~' && (!exp[1] || exp[1] == '$' || exp[1] == '|' || ft_is_there(AGG_REDI, exp[1])))))
        return (len = 1);
    while (exp[i] != '\0')
    {
        if ((i && exp[i] == '$') || (exp[i] == ';') || exp[i] == '|' || exp[i] == '/')
            break;
        if (is_blank(exp[i]) || ft_is_there(AGG_REDI, exp[i]) || (is_quote(exp[i])))
            break;
        else if (ft_isascii(exp[i]))
            len++;
        i++;
    }
    if (is_blank(exp[i]) || (!exp[i] && !ft_isalnum(exp[i - 1])))
        len++;
    return (len);
}

static char *ft_strjoin_until_char(char const *s1, char const *s2, char c)
{
    char *str;
    size_t lenstr;
    size_t new_strlen;

    lenstr = 0;
    str = NULL;
    new_strlen = ft_strlen_char_2((char *)s2, c, '$');
    if (s1 && s2)
    {
        lenstr = ft_strlen((char *)s1) + new_strlen;
        if (!(str = ft_strnew(lenstr)))
            return (NULL);
        str = ft_strcpy(str, s1);
        str = ft_strncat(str, s2, new_strlen);
        return (str);
    }
    return (NULL);
}

int ft_is_expansion(char *str)
{
    if (str)
    {
        if (str[0] == '$' && ft_isalnum(str[1]) && !is_quote(str[1]))
            return (1);
    }
    return (0);
}

int ft_is_tilde(char *str)
{
    if (str)
    {
        if (((str[0] == '~' && !is_quote(str[1]))) ||
            (str[0] == '~' && str[1] == 47))
            return (1);
    }
    return (0);
}

char *get_value_expansion(char *env_name, t_env **head)
{
    char *env_value;
    t_env *curr;

    curr = NULL;
    env_value = NULL;
    if (head)
        curr = *head;
    if (env_name && curr)
    {
        while (curr)
        {
            if (ft_strequ(curr->env_var_name, env_name))
                return ((env_value = ft_strdup(curr->env_var_value)));
            curr = curr->next;
        }
    }
    return (env_value);
}

static void after_exp(t_expansion *v, t_pointt *cor, t_lexer **token_node, t_env **env_list)
{
    if (v->buf[v->i] == '$')
        cor->no_space = 1;
    v->env_value = get_value_expansion(v->data, env_list);
    ft_strdel(&v->data);
    if (v->buf[v->i] && v->buf[v->i] != '$' && !ft_is_there(AGG_REDI, v->buf[v->i]) && v->buf[v->i] != '|' && !is_blank(v->buf[v->i]) && v->buf[v->i] != ';' && !is_quote(v->buf[v->i]))
    {
        v->data = v->env_value;
        v->env_value = ft_strjoin_until_char(v->env_value, v->buf + v->i, ' ');
        v->data_size += ft_strlen_char_2(v->buf + v->i, ' ', '$');
        if (v->buf[v->data_size - 1] == '$')
            cor->no_space = 1;
        ft_strdel(&v->data);
    }
    if (v->env_value)
        append_list(token_node, v->env_value, EXPANSION, cor);
    ft_strdel(&v->env_value);
}

static int exp_tild(t_expansion *v, t_lexer **token_node, t_env **env_list, t_pointt *cor)
{
    tilde_exp(v->buf + v->i, &v->env_value, env_list);
    if (v->env_value)
        append_list(token_node, v->env_value, EXPANSION, cor);
    ft_strdel(&v->env_value);
    ft_strdel(&v->data);
    return (ft_strlen_delim(v->buf, " ;$|><\n\t"));
}

int expansion_parse(t_lexer **token_node, char *buf, t_env **env_list, t_pointt *cor)
{
    t_expansion v;

    ft_bzero(&v, sizeof(t_expansion));
    cor->no_space = 0;
    if (buf && *buf && (v.data_size = get_size_expansion(buf)) > 0)
    {
        if (!(v.data = ft_strnew(v.data_size)))
            return (-1);
        if (buf[v.i] == '$')
            buf++;
        v.j = 0;
        v.buf = buf;
        while (buf[v.i] && (ft_isalnum(buf[v.i]) || (buf[v.i] == '~' && v.i == 0)) && v.i < v.data_size)
        {
            if (v.i == 0 && (ft_is_tilde(buf + v.i) || (buf[v.i] == '~')))
                return (exp_tild(&v, token_node, env_list, cor));
            v.data[v.j++] = buf[v.i];
            v.i++;
        }
        after_exp(&v, cor, token_node, env_list);
        return (v.data_size);
    }
    return (v.data_size + 1);
}
