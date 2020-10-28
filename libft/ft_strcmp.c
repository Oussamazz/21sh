/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 19:31:49 by oelazzou          #+#    #+#             */
/*   Updated: 2019/03/31 00:53:04 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	while (*(unsigned char *)s1 == *(unsigned char *)s2)
	{
		if (*s1 == '\0' || *s2 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
