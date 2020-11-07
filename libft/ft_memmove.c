/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 16:36:47 by oelazzou          #+#    #+#             */
/*   Updated: 2019/05/19 17:35:57 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			*ft_memmove(void *dest, const void *src, size_t n)
{
	const char	*psrc;
	char		*pdest;

	psrc = src;
	pdest = dest;
	if (pdest < psrc)
	{
		dest = ft_memcpy(dest, src, n);
		return (dest);
	}
	else
	{
		pdest = dest + n - 1;
		psrc = src + n - 1;
		while (n--)
		{
			*pdest-- = *psrc--;
		}
		return (dest);
	}
	return (dest);
}
