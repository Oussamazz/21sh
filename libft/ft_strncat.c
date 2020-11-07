/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 10:50:06 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/15 09:59:25 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	size_t	s1len;
	char	*ps1;

	s1len = ft_strlen((const char *)s1);
	ps1 = s1 + s1len;
	while (n > 0 && *s2)
	{
		*ps1++ = *s2++;
		n--;
	}
	*ps1 = '\0';
	return (s1);
}
