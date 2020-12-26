/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerCore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 13:15:48 by macos             #+#    #+#             */
/*   Updated: 2020/12/26 03:20:47 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char *get_splitter(char *buf, t_mystruct *v)
{
    int position;
    if (*buf == ';')
	{
		if ((position = sep_function(buf, &v->tokenz, &v->coord)) > 0)
			return(buf + position);
		ft_free_tokenz(&v->tokenz);
        return(NULL);
	}
    return buf;
}

static char *get_tild_dolar(char *buf, t_mystruct *v)
{
	int position;

	if ((*buf == '$' || *buf == '~') && !(*buf == '$' && buf[1] == '/') && (*buf != buf[1]) && !is_quote(buf[1])) //-> int expansion_function(char *, t_lexer **, t_pointt *, t_env **)
	{
		if ((position = expansion_function(buf, &v->tokenz, &v->coord, v->env_list)) > 0)
			return(buf + position);
		ft_free_tokenz(&v->tokenz);
		return (NULL);
	}
	return (buf);
}

static char *get_pipe_agr(char *buf, t_mystruct *v)
{
	int position;

	position = 0;
	if (*buf && ft_is_there(PIPE, *buf)) //-> int    pipe_function (char *, t_pointt *, t_lexer **)
	{
        append_list_pipe(&v->tokenz, ft_strdup("|"), PIPE_SYM, &v->coord);
		return (buf + 1);
	}
	else if (*buf && ft_is_there(AGG_REDI, *buf)) //-> void  aggr_functioin(char *, t_pointt *, t_lexer **, int *)            ||&& !check_quoting(&token_node, SQUOT, coord->aggr_index) && !check_quoting(&token_node, DQUOT, coord->aggr_index)
	{
		if (!v->tokenz)
        {
			print_error_sym(AGGR_SYM);
			return (NULL);
        }
		if ((position = aggr_function(buf, &v->coord, &v->tokenz)) == -1)
			return (NULL);
		return (buf + position);
	}
	return (buf);
}

static char *get_qoute_word(char *buf, t_mystruct *v)
{
	int position;

	if (is_quote(*buf)) //->      int     quote_function(char *buf, t_lexer **,t_pointt *, t_env **env_list)
	{
		position = quote_function(buf, &v->tokenz, &v->coord);
		if (position < 0)
		{
			if (!g_clt_c)
				ft_putstr_c_str("21sh: unexpected EOF while looking for matching `", *buf, "\'\n", 2);
			return (NULL);
		}
		return (buf + position);
	}
	if (*buf && !ft_is_there(METACHARACTER, *buf)) // word
	{
		if (is_quote(v->c = valid_string_quot(buf)) && v->c) // before quote " or ' joining
		{
			if (!(v->quot = quote_handling(buf, v->c, 0))) //->    int     quote_handling_function(t_lexer **, t_quote *, char quote, t_pointt *)
				return (NULL);
		    buf += quote_handling_function(&v->tokenz, v->quot, v->c, &v->coord);
		}
		else if (buf && *(buf)) // simple command simple_word_function(char *,  t_lexer **, t_pointt *, size_t buf_len)
			buf += simple_word_function(buf, &v->tokenz, &v->coord, v->size);
	}
	return (buf);
}


t_lexer *lexer(char *buf, t_env **env_list, t_pointt *coord)
{
	t_mystruct v;
	
	ft_bzero(&v, sizeof(t_mystruct));
    v.env_list = env_list;
    v.coord = *coord;
	v.size = ft_strlen(buf);
	while (*buf)
	{
		while (*buf && is_blank(*buf))
			buf++;
		if ((*buf == ';' && buf[1] == ';') || (*buf == ';' && !v.tokenz))
			return ((t_lexer *)err_ret("21sh: parse error near `;'\n", NULL));
        if((buf = get_splitter(buf, &v)) == NULL)
            return(NULL);
        if((buf = get_tild_dolar(buf, &v)) == NULL)
            return(NULL);
        if((buf = get_pipe_agr(buf, &v)) == NULL)
            return(NULL);
        if((buf = get_qoute_word(buf, &v)) == NULL)
            return(NULL);
	}
	return (v.tokenz);
}