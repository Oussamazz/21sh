/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 20:26:33 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/02 22:52:56 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list *new_list;

	new_list = ft_memalloc(sizeof(t_list));
	if (new_list == NULL)
		return (NULL);
	if (content != NULL)
	{
		new_list->content = ft_memalloc(content_size);
		ft_memcpy(new_list->content, content, content_size);
		new_list->content_size = content_size;
	}
	else if (content == NULL)
	{
		new_list->content = NULL;
		new_list->content_size = 0;
	}
	new_list->next = NULL;
	return (new_list);
}
