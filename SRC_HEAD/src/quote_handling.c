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

//echo "sdfsf'dfds fd"adfsdf'asdfasdf
//haha'

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

//echo "sdfsf'dfds fd"adfsdf'asdfasdf
//haha'

char		*get_content_quote(char *buffer, char c, t_pointt *coord, int flag_c)
{
	int i;
	char *str;
	int	flag;
	int len;

	str = NULL;
	if (buffer)
	{
		flag = flag_c; // 
		i = 0;
		len = get_content_len(buffer, c);
		if (!(str = ft_strnew(len)))
			return (NULL);
		len++;
		while (*buffer && len--)
		{
			if ((*buffer == c && !flag && is_quote(*(buffer + 1)))
				|| (*buffer == c && ft_isalnum(*(buffer + 1))))
				coord->no_space = 1;
			if ((*buffer == c && flag) ||
				(is_blank(*buffer) && flag) ||
				(is_quote(*buffer) && flag))
					break ;
			if (*buffer == c)
				flag = 1;
			else
				str[i++] = *buffer;
			buffer++;
		}
	}
	return (str);
}
