/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   his.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:44:38 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 12:42:05 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void			add_to_his(char *buffer, t_his **his)
{
	t_his		*last;
	t_his		*ret;

	if (buffer && !*buffer)
		return ;
	last = *his;
	if (!(ret = (t_his *)ft_memalloc(sizeof(t_his))))
		return ;
	if (!(ret->data = ft_strdup(buffer)))
		return ;
	if (last == NULL)
	{
		*his = ret;
		return ;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = ret;
}

char			*join_all_bufs(t_his *his)
{
	char		*ret;
	char		*tmp;

	ret = NULL;
	if (his)
	{
		ret = ft_strdup("");
		while (his)
		{
			tmp = ret;
			if (!(ret = ft_strjoin(ret, his->data)))
				return (NULL);
			ft_strdel(&tmp);
			his = his->next;
		}
	}
	return (ret);
}
