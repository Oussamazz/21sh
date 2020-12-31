/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 01:33:51 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int			split_agg(t_split_redir *v, char *str)
{
	if ((str[v->i] == '>' && str[v->i + 1] == '<') || (str[v->i] == '<' &&
		str[v->i + 1] == '>') || str[v->i + 1] == ';' || !str[v->i + 1])
	{
		ft_free_arr(v->agg);
		err_ret("21sh: syntax error near unexpected token `> or <'\n", NULL);
		return (ReturnNull);
	}
	v->agg[v->j][0] = str[v->i];
	v->active_word = 0;
	if (v->i + 1 < v->len && str[v->i + 1] == '&')
	{
		v->agg[v->j][1] = '&';
		if (v->i + 2 < v->len && str[v->i + 2] == '-')
		{
			v->agg[v->j][2] = '-';
			v->j++;
			return (Break);
		}
		v->i++;
	}
	v->j++;
	return (Normal);
}

static int			split_herdoc2(t_split_redir *v, char *str)
{
	char *delim;
	char *text;

	delim = ft_strndup(str + v->i, ft_strlen_char2(str + v->i, ' ', ';'));
	text = NULL;
	if (delim)
		text = here_doc(delim);
	if (text == NULL)
		return (ReturnNull);
	if (text)
	{
		free(v->agg[v->j]);
		v->agg[v->j++] = text;
		v->active_word = 1;
	}
	v->delim_len = ft_strlen(delim);
	if (str[v->i + v->delim_len - 1] == ';')
		v->delim_len--;
	ft_strdel(&delim);
	return (Break);
}

static int			split_redir_fd(t_split_redir *v, char *str)
{
	if (ft_isascii(str[v->i + 1]) && str[v->i + 1] != '<' &&
		str[v->i + 1] != '>' && str[v->i + 1] != '-')
	{
		ft_free_arr(v->agg);
		err_ret("21sh: syntax error near unexpected token `&'\n", NULL);
		return (ReturnNull);
	}
	v->agg[v->j][0] = str[v->i];
	if (v->i + 1 < v->len && (str[v->i + 1] == '>' || str[v->i + 1] == '<'))
	{
		v->agg[v->j][1] = str[v->i + 1];
		v->active_word = 0;
		if (v->i + 2 < v->len && (str[v->i + 2] == str[v->i + 1] ||
			str[v->i + 2] == '-'))
		{
			v->agg[v->j][2] = str[v->i + 2];
			v->j++;
			return (Break);
		}
		v->i++;
	}
	v->j++;
	return (Normal);
}

static int			do_spliting(t_split_redir *v, char *str)
{
	while (str[v->i] && is_blank(str[v->i]) && ++v->i)
		g_agg_len++;
	if (str[v->i] == ';')
		return (Returnagg);
	if (!(v->agg[v->j] = ft_strnew(v->len)) || v->i >= v->len)
		return (ReturnNull);
	if ((str[v->i] == '>' || str[v->i] == '<') && str[v->i] == str[v->i + 1])
		return (split_herdoc(v, str));
	if (str[v->i] == '&')
		return (split_redir_fd(v, str));
	if ((str[v->i] == '>' || str[v->i] == '<') || str[v->i + 1] == '&')
		return (split_agg(v, str));
	if (is_quote(str[v->i]))
	{
		ft_strdel(&v->agg[v->j]);
		return (Returnagg);
	}
	if ((ft_isalnum(str[v->i])) && v->j > 0 &&
		ft_strequ(v->agg[v->j - 1], "<<") && !is_quote(str[v->i]))
		return (split_herdoc2(v, str));
	if (ft_isascii(str[v->i]) && (v->i < v->len) && !v->active_word)
		return (split_varname(v, str));
	if (ft_isascii(str[v->i]) && v->active_word)
		return (Break);
	return (Normal);
}

char				**split_redir(char *str)
{
	t_split_redir	v;

	ft_bzero(&v, sizeof(t_split_redir));
	v.agg_len = 3;
	if (str && (v.agg = (char **)ft_memalloc(sizeof(char *) * v.agg_len)))
	{
		v.len = ft_strlen(str);
		while (v.i < v.len && str[v.i] != '\0' && v.j < v.agg_len)
		{
			v.status = do_spliting(&v, str);
			if (v.status == Returnagg)
				return (v.agg);
			if (v.status == ReturnNull)
				return (NULL);
			if (v.status == Break)
				break ;
			if (v.status == Continue)
				continue ;
			v.i++;
		}
	}
	v.agg[v.j] = NULL;
	g_agg_len += countall_arr(v.agg, v.delim_len);
	return (v.agg);
}
