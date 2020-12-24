/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 09:38:58 by macos             #+#    #+#             */
/*   Updated: 2020/12/24 19:32:50 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

size_t get_list_size(t_lexer *tokenz)
{
    size_t size;

    size = -1;
    if (tokenz)
    {
        size++;
        while (tokenz)
        {
            if (tokenz->data)
                size++;
            tokenz = tokenz->next;
        }
    }
    return (size);
}

size_t      ft_strlen_char2(char *s, char c, char c2)
{
    size_t len;

    len = 0;
    while (s[len] != c && s[len] != c2 && !ft_is_there(AGG_REDI, s[len]) && s[len] != '|')
        len++;
    if (s[len] == c2 || s[len] == '|' || ft_is_there(AGG_REDI, s[len]))
        g_agg_len--;
    return (len);
}

int     env_exist(t_env **env_list, char *var_name)
{
    t_env *current;

    if (var_name && *env_list)
    {
        current = *env_list;
        while (current)
        {
            if (ft_strequ(current->env_var_name, var_name))
                return (1);
            current = current->next;
        }
        return (0);
    }
    return (-1);
}

void    modify_env(t_env **env_list, char *var_name, char *var_value)
{
    t_env *current;

    if (var_name && var_value && *env_list)
    {
        current = *env_list;
        while (current)
        {
            if (ft_strequ(current->env_var_name, var_name))
            {
                ft_strdel(&current->env_var_value);
                current->env_var_value = ft_strdup(var_value);
                return ;
            }
            current = current->next;
        }
    }
}

char			*get_cwd(void)
{
	char	tmp[MAX_INDEX];
	char	*buff;

	buff = ft_strnew(MAX_INDEX);
	if (!buff)
		return (NULL);
	if (!getcwd(tmp, sizeof(tmp)))
		ft_strdel(&buff);
	else
		ft_strcpy(buff, tmp);
	return (buff);
}

void			gen_oldpwd(char *cwd, t_env **env_list)
{
	bool	flag;
    char    *cwd_;
    char    *cmd[4]; // setenv OLDPWD Valuee..... NULL

	flag = false;
    cwd_ = NULL;
	if (cwd == NULL)
	{
		if (!(cwd_ = get_value_expansion("PWD", env_list)))
            return ;
        cwd = cwd_;
		flag = true;
	}
    cmd[0] = "setenv";
    cmd[1] = "OLDPWD";
    cmd[2] = cwd;
    cmd[3] = NULL;
	blt_setenv(cmd, env_list);
	if (flag)
		ft_strdel(&cwd_);
    return ;
}

void			gen_pwd(char *new_path, t_env **env_list)
{
    char *cmd[4];

    if (new_path == NULL)
        return ;
    cmd[0] = "setenv";
    cmd[1] = "PWD";
    cmd[2] = new_path;
    cmd[3] = NULL;
    blt_setenv(cmd, env_list);
    return ;
}

int     str_is_blank(char *buffer)
{
    while (*buffer)
    {
        if (!is_blank(*buffer))
            return (0);
        buffer++;
    }
    return (1);
}