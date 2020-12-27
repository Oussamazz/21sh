/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_until.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 11:17:25 by oelazzou             #+#    #+#             */
/*   Updated: 2020/11/20 11:17:29 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_print_until(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
        else
            ft_putchar_fd(*s, 1);
		s++;
	}
	if ((char)c == '\0' && *s == '\0')
		return ((char *)s);
	return (NULL);
}
