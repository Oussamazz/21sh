/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:38:24 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 16:34:03 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

char			*get_content_quote(char *buffer, char c,
	t_pointt *coord, int flag)
{
	const char	*tmp;
	char		*str;

	tmp = buffer;
	if (buffer)
	{
		while (*buffer != c)
			buffer++;
		str = ft_strsub(tmp, 0, buffer - tmp);
		if (!flag)
			flag = !is_blank(buffer[1]);
		coord->no_space = flag;
		return (str);
	}
	return (NULL);
}
