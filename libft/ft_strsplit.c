/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 04:36:34 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/26 03:13:56 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			word_count(char *s, char c)
{
	int				i;
	int				res;

	i = 0;
	res = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && s[i + 1] == c)
			res++;
		i++;
	}
	if (s[i] == '\0' && s[i - 1] != c)
		res++;
	return (res);
}

static int			word_len(char *s, char c)
{
	int				len;

	len = 0;
	while (*s != c && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

char				**ft_strsplit(char const *s, char c)
{
	int			word_countx;
	char		**str;
	int			i;

	if (!s || !*s)
		return (NULL);
	word_countx = word_count((char *)s, c);
	if (!(str = (char**)ft_memalloc(sizeof(char*) * word_countx + 1)))
		return (NULL);
	i = 0;
	if (!str)
		return (str);
	while (word_countx-- && *s)
	{
		while (*s == c && *s != '\0')
			s++;
		if (!(str[i] = ft_strsub((char *)s, 0, word_len((char*)s, c))))
			return (str);
		if (!str[i])
			return (str);
		s = s + word_len((char *)s, c);
		i++;
	}
	str[i] = NULL;
	return (str);
}
