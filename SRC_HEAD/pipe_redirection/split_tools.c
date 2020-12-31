/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:31:50 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char		*redirection_varname(char ***arr, char *str, size_t *i)
{
	size_t	c_len;
	char	**agg;
	char	*tmp;

	tmp = NULL;
	c_len = 0;
	agg = *arr;
	while (!is_blank(str[*i + c_len]) &&
		str[*i + c_len] != ';' && str[*i + c_len] != '|' &&
			str[*i + c_len] && !ft_is_there(AGG_REDI, str[*i + c_len]))
		c_len++;
	if (str[*i + c_len] == '&')
		error_message("21sh: Error[1]: control jobs not handled yet...\n", 1);
	tmp = ft_strsub(str, *i, c_len);
	*i = *i + c_len;
	return (tmp);
}

size_t		countall_arr(char **agg, size_t delime_len)
{
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (agg)
	{
		while (agg[i])
		{
			if (i > 0 && ft_strequ(agg[i - 1], "<<"))
				len = (len - ft_strlen(agg[i])) + delime_len + 1;
			len = len + ft_strlen(agg[i]);
			i++;
		}
		return (len);
	}
	return (-1);
}

int			split_varname(t_split_redir *v, char *str)
{
	ft_strdel(&v->agg[v->j]);
	v->agg[v->j] = redirection_varname(&v->agg, str, &v->i);
	v->j++;
	v->active_word = 1;
	return (Break);
}

int			split_herdoc(t_split_redir *v, char *str)
{
	if ((v->i + 2 < v->len) && ((str[v->i + 2] == str[v->i]) ||
		(str[v->i + 2] == ';' || str[v->i + 2] == '|')))
	{
		ft_free_arr(v->agg);
		err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
		return (ReturnNull);
	}
	v->agg[v->j][0] = str[v->i];
	v->agg[v->j][1] = str[v->i];
	v->active_word = 0;
	v->j++;
	v->i = v->i + 2;
	return (Continue);
}
