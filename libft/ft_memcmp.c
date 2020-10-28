/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 17:44:34 by oelazzou          #+#    #+#             */
/*   Updated: 2019/03/30 04:30:20 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*ps1;
	const unsigned char	*ps2;

	ps1 = s1;
	ps2 = s2;
	while (n--)
	{
		if (*ps1 != *ps2)
		{
			return (*ps1 - *ps2);
		}
		ps1++;
		ps2++;
	}
	return (0);
}
