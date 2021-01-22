/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   his.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:44:38 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/22 11:22:52 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

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
