/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 18:25:32 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/15 10:05:42 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char *ps;

	ps = (char *)s + ft_strlen(s);
	while (ps >= s)
	{
		if (*ps == (char)c)
			return (ps);
		ps--;
	}
	return (NULL);
}
