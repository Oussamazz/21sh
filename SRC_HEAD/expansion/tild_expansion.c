/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tild_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 14:40:33 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		tild_slash(char *exp, char **data, t_tilde_exp *v)
{
	v->i = 0;
	while (exp[v->i + 1] == 47)
		v->i++;
	if (ft_isalnum(exp[v->i + 1]) && exp[v->i] == 47)
	{
		v->user_name = ft_strsub(exp + 1, 0, ft_strlen_char(exp + 1, ' '));
		if (v->user_name)
		{
			*data = ft_strjoin(v->home_value, v->user_name);
			ft_strdel(&v->home_value);
			ft_strdel(&v->user_name);
			return ;
		}
	}
	else
		*data = v->home_value;
	ft_strdel(&v->user_name);
}

void			tilde_exp(char *exp, char **data, t_env **env_list)
{
	t_tilde_exp	v;

	ft_bzero(&v, sizeof(t_tilde_exp));
	v.home_value = get_value_expansion("HOME", env_list);
	if (v.home_value == NULL)
	{
		v.user = get_value_expansion("USER", env_list);
		if (!(v.home_value = ft_strjoin("/Users/", v.user)))
			return (ft_putendl_fd("HOME and USER are unset.", 2));
		ft_strdel(&v.user);
	}
	if (exp[0] == '~' && exp[1] != 47 && !ft_is_there(AGG_REDI, exp[1]) &&
		!is_blank(exp[1]) && exp[1] != '|' && exp[1] != ';' && exp[1] != '$')
	{
		*data = ft_strdup(exp);
		ft_strdel(&v.home_value);
		ft_strdel(&v.user_name);
		return ;
	}
	if (exp[1] == 47)
		return (tild_slash(exp, data, &v));
	else
		*data = v.home_value;
	return ;
}
