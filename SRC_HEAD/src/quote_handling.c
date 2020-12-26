/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:38:24 by macos             #+#    #+#             */
/*   Updated: 2020/12/26 15:21:36 by oelazzou         ###   ########.fr       */
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
	int i = 0;
	int j = 0;
	bool flag = false;
	char *tmp;

	tmp = NULL;
	quot = NULL;
	rec_quote = NULL;
	if (!(quot = (t_quote *)ft_memalloc(sizeof(t_quote))))
		return (NULL);
	if (!(quot->string = ft_strnew(ft_strlen(s))))
		return (NULL);
	if (*s == quote)
		return (quot = init(&quot));
	//ft_bzero(&v,sizeof(t_mystruct));
	if (!*s)
		return (quote_completion(&quot, quote));
	while (*s)
	{
		if (s[i] != quote && start)
		{
			quot->string[j++] = s[i];
			flag = false;
		}
		// else if (s[i] == '\\' && (i == 0 || s[i - 1] != '\\')) // quote != '\''
		// {
		// 	if (s[i + 1] != '\"' && s[i + 1] != '\'' && s[i + 1] != '$' && start)
		// 		quot->string[j++] = s[i];
		// 	flag = true;
		// }
		else if (is_quote(s[i]) && flag == false)
		{
			if (s[i + 1] == '>' || s[i + 1] == '<' || s[i + 1] == '|')
				i = i + (ft_strchr(s + i, quote) - (s + i));
			else if ((!is_blank(s[i + 1]) || s[i + 1] == '\n') && (!ft_is_there(METACHARACTER, s[i + 1]) || s[i + 1] == '\n'))
			{
				//ft_putendl_fd(s + i, 1);
				if (!(rec_quote = quote_handling(s + i + 1, s[i], !start)))
					return (NULL);
				tmp = quot->string;
				quot->string = ft_strjoin(quot->string, rec_quote->string);
				i = i + rec_quote->size + 1;
				ft_strdel(&(rec_quote->string));
				ft_memdel((void **)&rec_quote);
				ft_strdel(&tmp);
				break;
			}
			else if (s[i] != quote || !start)
				return (quote_completion(&quot, s[i]));
			i++;
			break;
		}
		else
		{
			quot->string[j++] = s[i];
			if (s[i + 1] == '>' || s[i + 1] == '<')
			{
				i++;
				break;
			}
			flag = false;
		}
		if (!s[i + 1] && start)
			return (quote_completion(&quot, quote));
		i++;
	}
	quot->size = i;
	return (quot);
}
