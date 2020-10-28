/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 21:03:44 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/08 21:15:06 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char		*ft_strndup(const char *s1, size_t n)
{
	char	*str;
	int		i;

	str = (char *)malloc(sizeof(*str) * (n + 1));
	if (str == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0' && n)
	{
		str[i] = s1[i];
		i++;
		n--;
	}
	str[i] = '\0';
	return (str);
}
