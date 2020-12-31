/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envirement.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/15 15:30:12 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			addtolist(t_env **head, char *var_name, char *var_value)
{
	t_env	*tmp;
	t_env	*new;

	if (!(new = (t_env *)ft_memalloc(sizeof(t_env))))
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

char		**list_to_tabs(t_env **env_list)
{
	char	**tabs;
	char	*str;
	t_env	*current;
	int		i;

	if (!(current = *env_list))
		return (NULL);
	tabs = NULL;
	i = -1;
	while (++i >= 0 && current != NULL)
		current = current->next;
	if (!(tabs = (char **)ft_memalloc(sizeof(char *) * (i + 1))))
		return (NULL);
	current = *env_list;
	i = 0;
	while (current != NULL)
	{
		if (current->env_var_name && current->env_var_value)
		{
			str = ft_strjoin(current->env_var_name, "=");
			tabs[i++] = ft_freejoin(str, current->env_var_value, 0);
		}
		current = current->next;
	}
	return (tabs);
}

void		stock_env(char **env, t_env **head)
{
	char	*var_value;
	char	*var_name;
	int		i;
	size_t	len;

	if (!env || !*env)
	{
		ft_putendl_fd_error("\033[1;33m", ERROR3, "\n", "\033[0m");
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
