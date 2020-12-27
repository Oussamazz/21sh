/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:38:24 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/27 01:10:42 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_quote *init(t_quote **quot)
{
	if (quot)
	{
		ft_strdel(&(*quot)->string);
		(*quot)->string = ft_strdup("");
		(*quot)->size = 1;
	}
	return (*quot);
}

t_quote *quote_handling(char *s, char quote, int start)
{
	//t_mystruct v;
	t_quote *quot;
	t_quote *rec_quote;
	int i;
	int j;
	int flag;
	char *tmp;
	size_t len;

	tmp = NULL;
	quot = NULL;
	rec_quote = NULL;
	len = ft_strlen(s);
	if (!(quot = (t_quote *)ft_memalloc(sizeof(t_quote))))
		return (NULL);
	if (!(quot->string = ft_strnew(len)))
		return (NULL);
	quot->size = 0;
	if (!*s)
		return (quote_completion(&quot, quote));
	j = 0;
	i = 0;
	flag = 0;
	while (*s && len--)
	{
		if (s[i] != quote && !flag)
		{
			quot->string[j++] = s[i];
			flag = 0;
		}
		else if (s[i] == quote && flag == 0 && start)
		{
			flag = 1;
			start = 0;
			if (s[i + 1] && (!is_blank(s[i + 1]) || s[i + 1] == '\n') &&
				(s[i + 1] != ';' || s[i + 1] == '\n') && s[i + 1] != '|' &&
				!ft_is_there(AGG_REDI, s[i + 1]))
			{
				if (is_quote(s[i + 1]))
					start = 1;
				else
					start = 0;
				if (start)
				{
					if (!(rec_quote = quote_handling(s + i + 2, s[i + 1], start)))
						return (NULL);
				}
				else
				{
					if (!(rec_quote = quote_handling(s + i + 1, quote, start)))
						return (NULL);
				}
				tmp = quot->string;

				if (rec_quote->string)
					quot->string = ft_strjoin(quot->string, rec_quote->string);
				i = i + rec_quote->size + 1;
				ft_strdel(&(rec_quote->string));
				ft_memdel((void **)&rec_quote);
				ft_strdel(&tmp);
				break;
			}
			i += 1;
			break;
		}
		else if (is_quote(s[i]) && start == 0 && s[i] == quote)
		{
			if (!(rec_quote = quote_handling(s + i + 1, s[i], start)))
				return (NULL);
			tmp = quot->string;
			if (rec_quote->string)
				quot->string = ft_strjoin(quot->string, rec_quote->string);
			i = i + rec_quote->size + 1;
			ft_strdel(&(rec_quote->string));
			ft_memdel((void **)&rec_quote);
			ft_strdel(&tmp);
			i++;
			break;
		}
		if (!s[i + 1] && start)
			return (quote_completion(&quot, quote));
		i++;
	}
	quot->size = i;
	return (quot);
}