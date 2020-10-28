/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 11:07:37 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/15 09:59:19 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t srclen;
	size_t destlen;
	size_t w;

	srclen = ft_strlen(src);
	destlen = ft_strlen((const char *)dest);
	if (destlen >= size)
		srclen = srclen + size;
	else
		srclen = srclen + destlen;
	w = 0;
	while (src[w] != '\0' && destlen + 1 < size)
	{
		dest[destlen] = src[w];
		destlen++;
		w++;
	}
	dest[destlen] = '\0';
	return (srclen);
}
