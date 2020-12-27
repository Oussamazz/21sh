/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/15 15:30:12 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/14 23:29:17 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int addtolist(t_env **head, char *var_name, char *var_value)
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

char	**list_to_tabs(t_env **env_list)
{
	char	**tabs;
    char    *str;
	t_env	*current;
	int		i;

	current = *env_list;
	tabs = NULL;
	i = -1;
	if (!current)
		return (NULL);
	while (++i >= 0 && current != NULL)
		current = current->next;
	if (!(tabs = (char **)ft_memalloc(sizeof(char*) * (i + 1))))
		return (tabs);
	current = *env_list;
	i = 0;
	while (current != NULL)
	{
		if (current->env_var_name && current->env_var_value)
        {
			str = ft_strjoin(current->env_var_name, "=");
            tabs[i++] = ft_strjoin(str, current->env_var_value);
            ft_strdel(&str);
        }
		current = current->next;
	}
	tabs[i] = NULL;
	return (tabs);
}

void    stock_env(char **env, t_env **head)
{
    char    *var_value;
    char    *var_name;
    int i;
    size_t len;

    if (!env || !*env)
	{
		ft_putendl_fd_error("\033[1;33m","Envirenment variables not set.","\n","\033[0m");
		exit(1);
	}
    i = 0;
    while (env[i] != NULL)
    {
        len = ft_strlen_char(env[i], '=');
        var_name = ft_strsub(env[i], 0, len);
        var_value = ft_strsub(env[i], len + 1, ft_strlen(env[i]) - len - 1);
        addtolist(head, var_name, var_value);
        i++;
    }
    return ;
}
