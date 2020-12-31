/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 18:48:56 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		ft_free_arr(char **arr)
{
	int		i;

	if (arr)
	{
		i = 0;
		while (arr[i] != NULL)
		{
			ft_strdel(&arr[i]);
			i++;
		}
		if (arr)
			free(arr);
		arr = NULL;
	}
}

void		free_env_list(t_env **head)
{
	t_env	*cur;
	t_env	*tmp;

	cur = *head;
	while (cur)
	{
		tmp = cur->next;
		ft_strdel(&cur->env_var_name);
		ft_strdel(&cur->env_var_value);
		free(cur);
		cur = tmp;
	}
}
