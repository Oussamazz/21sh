/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 16:34:19 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

bool			is_quote(int c)
{
	if (c == '\'' || c == '\"')
		return (true);
	return (false);
}

size_t			ft_strlen_char(char *s, char c)
{
	size_t		len;

	len = 0;
	while (s[len] != '\0' && s[len] != c)
		len++;
	return (len);
}

size_t			ft_strlen_char_2(char *s, char c, char c2)
{
	size_t		len;

	len = 0;
	while (s[len] != '\0' && s[len] != c && s[len] != c2)
		len++;
	return (len);
}

size_t			ft_strlen_delim(char *s, char *delim)
{
	size_t		len;

	len = 0;
	while (s[len] != '\0' && !ft_is_there(delim, s[len]))
		len++;
	return (len);
}

char			*ft_strchr_blank(const char *str, int c)
{
	int			i;
	char		*s;

	i = 0;
	s = (char *)str;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (&s[i]);
		else if (is_blank(s[i]))
			return (NULL);
		i++;
	}
	if ((char)c == '\0' && s[i] == '\0')
		return (&s[i]);
	return (NULL);
}
