/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstrrchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/19 14:20:54 by oelazzou          #+#    #+#             */
/*   Updated: 2019/09/23 13:17:17 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr_path(const char *s, int c)
{
	char *ps;

	ps = (char *)s + ft_strlen(s);
	while (ps >= s)
	{
		if (*ps == (char)c)
			return (ps + 1);
		ps--;
	}
	return ((char*)s);
}
