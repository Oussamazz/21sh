/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 09:38:58 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

size_t			get_list_size(t_lexer *tokenz)
{
	size_t		size;

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

size_t			ft_strlen_char2(char *s, char c, char c2)
{
	size_t		len;

	len = 0;
	while (s[len] != c && s[len] != c2 &&
		!ft_is_there(AGG_REDI, s[len]) && s[len] != '|')
		len++;
	if (s[len] == c2 || s[len] == '|' || ft_is_there(AGG_REDI, s[len]))
		g_agg_len--;
	return (len);
}

int				env_exist(t_env **env_list, char *var_name)
{
	t_env		*current;

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

void			modify_env(t_env **env_list, char *var_name, char *var_value)
{
	t_env		*current;

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
	char		tmp[MAX_INDEX];
	char		*buff;

	buff = ft_strnew(MAX_INDEX);
	if (!buff)
		return (NULL);
	if (!getcwd(tmp, sizeof(tmp)))
		ft_strdel(&buff);
	else
		ft_strcpy(buff, tmp);
	return (buff);
}
