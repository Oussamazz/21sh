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
            break ;
        if (is_blank(exp[i]) || ft_is_there(AGG_REDI, exp[i]) || (is_quote(exp[i])))
            break ;
        else if (ft_isascii(exp[i]))
            len++;
        i++;
    }
    if (is_blank(exp[i]) || (!exp[i] && !ft_isalnum(exp[i - 1])))
        len++;
    return (len);
}

static char	*ft_strjoin_until_char(char const *s1, char const *s2, char c)
{
	char	*str;
	size_t	lenstr;
    size_t  new_strlen;

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

int         ft_is_expansion(char *str)
{
    if (str)
    {
        if (str[0] == '$' && ft_isalnum(str[1]) && !is_quote(str[1]))
            return (1);
    }
    return (0);
}

int     ft_is_tilde(char *str)
{
    if (str)
    {
        if (((str[0] == '~' && !is_quote(str[1]))) ||
         (str[0] == '~' && str[1] == 47))
            return (1);
    }
    return (0);
}

char        *get_value_expansion(char *env_name, t_env **head)
{
    char    *env_value;
    t_env   *curr;

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

static void tilde_exp(char *exp, char **data, t_env **env_list)
{
    int i;
    char *home_value;
    char *user;
    char *user_name;

    if (exp && *exp)
    {
        home_value = NULL;
        user = NULL;
        user_name = NULL;
        if (exp[0] == '~')
        {
            home_value = get_value_expansion("HOME", env_list);
            if (home_value == NULL) // home_value = ft_strjoin("/Users/$USER");
            {
                user = get_value_expansion("USER", env_list);
                if (!(home_value = ft_strjoin("/Users/", user)))
                    return (ft_putendl_fd("HOME and USER are unset.", 2));
                ft_strdel(&user);
            }
            if (exp[0] == '~' && exp[1] != 47 && !ft_is_there(AGG_REDI, exp[1]) &&
             !is_blank(exp[1]) && exp[1] != '|' && exp[1] != ';' && exp[1] != '$')
            {
                *data = ft_strdup(exp);
                ft_strdel(&home_value);
                ft_strdel(&user_name);
                return ;
            }
            if (exp[1] == 47)
            {
                i = 0;
                while (exp[i + 1] == 47)
                    i++;
                if (ft_isalnum(exp[i + 1]) && exp[i] == 47)
                {
                    if(!(user_name = ft_strsub(exp + 1, 0, ft_strlen_char(exp + 1, ' '))))
                        return ;
                    if (user_name)
                    {
                        *data = ft_strjoin(home_value, user_name);
                        ft_strdel(&home_value);
                        ft_strdel(&user_name);
                        return ;
                    }
                }
                else
                    *data = home_value;
                ft_strdel(&user_name);
            }
            else
                *data = home_value;
        }
    }
    return ;
}

int     expansion_parse(t_lexer **token_node, char *buf, t_env **env_list, t_pointt *cor)
{
    int     i;
    int     j;
    int     data_size;
    char    *data;
    char    *env_value;

    i = 0;
    env_value = NULL;
    data_size = 0;
    cor->no_space = 0;
    if (buf && *(buf + i))
    {
        if ((data_size = get_size_expansion(buf)) > 0)
        {
            if (!(data = ft_strnew(data_size)))
                return (-1);
            if (buf[i] == '$')
                buf++;
            j = 0;
            while (buf[i] && (ft_isalnum(buf[i]) || (buf[i] == '~' && i == 0)) && i < data_size)
            {
                if (i == 0 && (ft_is_tilde(buf + i) || (buf[i] == '~')))
                {
                    tilde_exp(buf + i, &env_value, env_list);
                    if (env_value)
                        append_list(token_node, env_value, EXPANSION, cor);
                    ft_strdel(&env_value);
                    ft_strdel(&data);
                    return (ft_strlen_delim(buf, " $|><\n\t"));
                }
                else
                    data[j++] = buf[i];
                i++;
            }
            if (buf[i] == '$')
                cor->no_space = 1;
            env_value = get_value_expansion(data, env_list);
            ft_strdel(&data);
            if (buf[i] && buf[i] != '$' && !ft_is_there(AGG_REDI, buf[i]) && buf[i] != '|' && !is_blank(buf[i]) && buf[i] != ';' && !is_quote(buf[i]))
            {
                data = env_value;
                if (!(env_value = ft_strjoin_until_char(env_value, buf + i, ' ')))
                    return (data_size);
                data_size += ft_strlen_char_2(buf + i, ' ', '$');
                if (buf[data_size - 1] == '$')
                    cor->no_space = 1;
                ft_strdel(&data);
            }
            if (env_value)
                append_list(token_node, env_value, EXPANSION, cor);
            ft_strdel(&env_value);
            return (data_size);
        }
    }
    return (data_size + 1);
}

