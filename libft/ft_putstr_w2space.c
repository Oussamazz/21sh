/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_w2space.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 09:26:12 by oelazzou          #+#    #+#             */
/*   Updated: 2019/09/17 09:27:21 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_w2space(const char *s1)
{
	if (s1)
	{
		ft_putstr(s1);
		ft_putstr("  ");
	}
	return ;
}
