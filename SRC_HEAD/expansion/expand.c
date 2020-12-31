/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 22:07:06 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		expansion_func(t_expand *v, t_env **head)
{
	if (v->tmp > v->arr[v->i] && *(v->tmp - 1) == '\\')
		v->i++;
	else if (!(v->value[v->j] = get_value_expansion(
			ft_strchr(v->arr[v->i], '$') + 1, head)))
		v->i++;
	else if (v->string)
	{
		v->string = ft_freejoin(v->string, v->value[v->j], 0);
		ft_strdel(&v->value[v->j]);
	}
	else
		v->string = v->value[v->j];
	v->j++;
}

static void		not_is_expansion(t_expand *v)
{
	if (v->string && !ft_is_expansion(v->arr[v->i]))
		v->string = ft_freejoin(v->string, v->arr[v->i], 0);
	if (!v->i && !v->j && !ft_is_expansion(v->arr[v->i]))
		v->string = ft_strdup(v->arr[v->i]);
}

char			*expanded(t_env **head, char *str)
{
	t_expand	v;

	ft_bzero(&v, sizeof(t_expand));
	if (!(v.arr = strsplit(str)))
		return (NULL);
	v.val_size = get_arr_size(v.arr) + 1;
	v.string = NULL;
	v.tmp = NULL;
	if (!(v.value = (char **)ft_memalloc(sizeof(char *) * v.val_size)))
		return (NULL);
	v.j = 0;
	v.i = 0;
	while (v.arr[v.i] != NULL && v.j < v.val_size && v.i < v.val_size)
	{
		if (ft_is_expansion((v.tmp = ft_strchr(v.arr[v.i], '$'))))
			expansion_func(&v, head);
		else
			not_is_expansion(&v);
		v.i++;
	}
	ft_free_arr(v.arr);
	free(v.value);
	v.value = NULL;
	return (v.string);
}

int				ft_is_expansion(char *str)
{
	if (str)
	{
		if (str[0] == '$' && ft_isalnum(str[1]) && !is_quote(str[1]))
			return (1);
	}
	return (0);
}

int				ft_is_tilde(char *str)
{
	if (str)
	{
		if (((str[0] == '~' && !is_quote(str[1]))) ||
			(str[0] == '~' && str[1] == 47))
			return (1);
	}
	return (0);
}
