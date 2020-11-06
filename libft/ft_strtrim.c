/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 01:18:12 by oelazzou          #+#    #+#             */
/*   Updated: 2019/05/20 04:55:11 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_white(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}

char		*ft_strtrim(char const *s)
{
	int				end;
	unsigned int	start;
	char			*str;

	if (s == NULL)
		return (NULL);
	start = 0;
	while (s[start] && is_white(s[start]))
		start++;
	end = (int)ft_strlen(s) - 1;
	while (s[end] && is_white(s[end]) && end > (int)start)
		end--;
	str = ft_strsub(s, start, (end - start + 1));
	if (str == NULL)
		return (NULL);
	return (str);
}
