/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:38:24 by macos             #+#    #+#             */
/*   Updated: 2020/12/26 18:56:13 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "21sh.h"

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
	int flag = 0;
	char *tmp;

	tmp = NULL;
	quot = NULL;
	rec_quote = NULL;
	if (!(quot = (t_quote *)ft_memalloc(sizeof(t_quote))))
		return (NULL);
	if (!(quot->string = ft_strnew(ft_strlen(s))))
		return (NULL);
	quot->size = 0;
	if (!*s)
		return (quote_completion(&quot, quote));
	j = 0;
	i = 0;
	while (*s)
	{
		if (s[i] != quote && flag == 0)
		{
			quot->string[j++] = s[i];
			flag = 0;
		}
		else if (s[i] == quote && flag == 0 && start) // "sdfdsf";
		{
			flag = 1;
			if (s[i + 1] && (!is_blank(s[i + 1]) || s[i + 1] == '\n') &&
			 (s[i + 1] != ';' || s[i + 1] == '\n') && s[i + 1] != '|')
			{
				if (is_quote(s[i + 1]))
					start = 1;
				else
					start = 0;
				if (start)
				{
					if (!(rec_quote = quote_handling(s + i + 1, s[i + 1], start)))
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
			else
				i += 1;
			break;
		}
		if (!s[i + 1] && start)
			return (quote_completion(&quot, quote));
		i++;
	}
	quot->size = i;
	return (quot);
}
