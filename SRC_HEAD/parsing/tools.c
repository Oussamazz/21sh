/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:15:23 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

size_t			get_arr_size_tokenz(t_lexer *token)
{
	size_t size;

	size = -1;
	if (token)
	{
		size++;
		while (token != NULL)
		{
			if (token->type != PIPE_SYM &&
				token->type != METACHAR && token->type != SEP)
				size++;
			else
				break ;
			token = token->next;
		}
	}
	return (size);
}

int				get_the_word(char *buf, t_lexer **token_node, t_pointt *coord)
{
	char	tmp[MIN_INDEX];
	int		j;

	j = 0;
	while (buf[j] && !ft_is_there(METACHARACTER, buf[j]) &&
		!ft_is_aggr(buf[j]) && buf[j] != '|' && buf[j] != '$')
	{
		tmp[j] = buf[j];
		j++;
	}
	if (buf[j] == '$')
		coord->no_space = 1;
	tmp[j] = '\0';
	append_list(token_node, tmp, WORD, coord);
	ft_strclr(tmp);
	return (j);
}

char			*get_dollars(char *buf)
{
	int			i;
	int			count;

	i = 0;
	count = 0;
	while (*buf == '$')
	{
		if (count == 2)
			break ;
		buf++;
		count++;
	}
	return (ft_itoa((int)getppid()));
}

char			*get_splitter(char *buf, t_mystruct *v)
{
	int			position;

	position = 0;
	if (*buf == ';')
	{
		if ((position = sep_function(&v->tokenz, &v->coord)) > 0)
			return (buf + position);
		ft_free_tokenz(&v->tokenz);
		return (NULL);
	}
	return (buf);
}
