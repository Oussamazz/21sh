/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 19:15:11 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/04 01:06:22 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t n)
{
	size_t i;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack && n)
	{
		if (*haystack == *needle)
		{
			i = 0;
			while (*(haystack + i) == *(needle + i)
			&& *(haystack + i) && *(needle + i) && i < n)
				i++;
			if (*(needle + i) == '\0')
				return ((char *)haystack);
		}
		haystack++;
		n--;
	}
	return (NULL);
}
