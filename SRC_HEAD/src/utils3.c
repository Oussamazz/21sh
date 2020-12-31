/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 17:38:56 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char			*get_left_fd_(char *buf)
{
	int			len;
	int			i;
	char		*ret;

	len = 0;
	while (ft_isdigit(buf[len]))
		len++;
	if (!len || !(ret = ft_strnew(len + 1)))
		return (NULL);
	i = 0;
	while (buf[i] && len)
	{
		ret[i] = buf[i];
		i++;
		len--;
	}
	return (ret);
}

int				ft_is_aggr(char c)
{
	if (c == '>' || c == '<')
		return (1);
	return (0);
}
