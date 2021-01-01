/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_no_blanks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 02:01:25 by macos             #+#    #+#             */
/*   Updated: 2020/12/29 15:25:09 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_no_blanks(char *s, int c, int c2)
{
	while (*s)
	{
		if (is_blank(*s))
			return (NULL);
		if (*s == (char)c || *s == (char)c2)
			return ((char *)s);
		s++;
	}
	if ((*s == (char)c || *s == (char)c2) && *s == '\0')
		return ((char *)s);
	return (NULL);
}
