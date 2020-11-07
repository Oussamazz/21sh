/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 01:59:07 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/07 01:40:49 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*next_lst;
	t_list	*list;

	list = *alst;
	while (list != NULL)
	{
		next_lst = list->next;
		del(list->content, list->content_size);
		ft_memdel((void**)&list);
		list = next_lst;
	}
	*alst = NULL;
}
