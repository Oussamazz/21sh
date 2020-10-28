/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 21:57:06 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/03 22:06:11 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(t_list *elem))
{
	t_list *next_lst;
	t_list *p;

	p = lst;
	while (p != NULL)
	{
		next_lst = p->next;
		f(p);
		p = next_lst;
	}
}
