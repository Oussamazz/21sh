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

static size_t	get_content_len(char *str, char c)
{
	size_t len;
	int		flag;

	len = 0;
	flag = 0;
	while (*str)
	{
		if (flag && (*str == ';' || *str== '|' || ft_is_there(AGG_REDI, *str)))
			break ;
		if ((is_blank(*str) && flag) || (is_quote(*str) && flag))
			break ;
		if (*str != c)
			len++;
		else
			flag = 1;
		str++;
	}
	return (len);
}

char		*get_content_quote(char *buffer, char c, t_pointt *coord, int flag)
{
	const char *tmp = buffer;
	char *str;

	if (buffer)
	{
		while (*buffer != c)
			buffer++;
		str = ft_strsub(tmp, 0, buffer - tmp);
		if(!flag)
			flag = !is_blank(buffer[1]);
		coord->no_space = flag;
		return(str);
	}
	return (NULL);
}
