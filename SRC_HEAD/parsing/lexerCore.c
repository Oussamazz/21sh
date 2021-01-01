/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexercore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:15:48 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 14:23:36 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char		*get_tild_dolar(char *buf, t_mystruct *v)
{
	int			position;
	char		*dollars;

	if (*buf == '$' && *(buf + 1) == '$')
	{
		append_list(&v->tokenz, (dollars = get_dollars(buf)), WORD, &v->coord);
		ft_strdel(&dollars);
		return (buf + 2);
	}
	if ((*buf == '$' || *buf == '~') && !(*buf == '$'
		&& buf[1] == '/') && (*buf != buf[1]) && !is_quote(buf[1]))
	{
		if (*buf == '$' && (!*(buf + 1) || is_blank(*(buf + 1))))
		{
			append_list(&v->tokenz, "$", WORD, &v->coord);
			return (buf + 1);
		}
		else if ((position = expansion_function(buf, &v->tokenz,
			&v->coord, v->env_list)) > 0)
			return (buf + position);
		ft_free_tokenz(&v->tokenz);
		return (NULL);
	}
	return (buf);
}

static char		*get_pipe_agr(char *buf, t_mystruct *v)
{
	int			position;

	position = 0;
	if (*buf && ft_is_there(PIPE, *buf))
	{
		append_list_pipe(&v->tokenz, ft_strdup("|"),
			PIPE_SYM, &v->coord);
		return (buf + 1);
	}
	else if (*buf && ft_is_there(AGG_REDI, *buf))
	{
		if (!v->tokenz)
		{
			print_error_sym(AGGR_SYM);
			return (NULL);
		}
		if ((position = aggr_function(buf, &v->coord, &v->tokenz)) == -1)
			return (NULL);
		return (buf + position + 1);
	}
	return (buf);
}

static char		*get_qoute_word(char *buf, t_mystruct *v)
{
	int			position;
	char		*quote;

	quote = NULL;
	if (is_quote(*buf) ||
		(*buf && (quote = ft_strchr_no_blanks(buf + 1, '\'', '\"'))))
	{
		position = quote_function(buf, &v->tokenz, &v->coord, quote);
		if (position < 0)
			return (NULL);
		return (buf + position);
	}
	if ((*buf &&
		!ft_is_there(METACHARACTER, *buf)) && *buf != '$' &&
		!ft_is_there(AGG_REDI, *buf) && *buf != '|')
	{
		if (buf && *(buf))
			buf += simple_word_function(buf, &v->tokenz, &v->coord);
	}
	return (buf);
}

static char		*ignore_blanks(char *str)
{
	while (is_blank(*str))
		str++;
	return (str);
}

t_lexer			*lexer(char *buf, t_env **env_list, t_pointt *coord)
{
	t_mystruct	v;

	ft_bzero(&v, sizeof(t_mystruct));
	v.env_list = env_list;
	v.coord = *coord;
	v.size = ft_strlen(buf);
	while (*buf)
	{
		buf = ignore_blanks(buf);
		if ((*buf == ';' && buf[1] == ';') || (*buf == ';' && !v.tokenz))
			return ((t_lexer *)err_ret("21sh: parse error near `;'\n", NULL));
		if ((buf = get_splitter(buf, &v)) == NULL)
			return (NULL);
		if ((buf = get_tild_dolar(buf, &v)) == NULL)
			return (NULL);
		if ((buf = get_pipe_agr(buf, &v)) == NULL)
			return (NULL);
		if ((buf = get_qoute_word(buf, &v)) == NULL)
			return (NULL);
	}
	return (v.tokenz);
}
