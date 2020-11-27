/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 19:11:38 by macos             #+#    #+#             */
/*   Updated: 2020/11/27 00:55:14 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int			word_count(char *s)
{
	int				i;
	int				res;

	i = 0;
	res = 0;
	while (s[i] != '\0')
	{
		if (ft_isascii(s[i]) && is_blank(s[i + 1]))
			res++;
        else if (is_blank(s[i]))
        {
            while (is_blank(s[i]))
                i++;
            continue ;
        }
		i++;
	}
    if (!s[i] && ft_isascii(s[i - 1]))
        res++;
	return (res);
}

static int	get_type(char c)
{
	if (is_blank(c))
		return (1);
	if (ft_isalnum(c))
		return (2);
	return (2);
}

static int			word_len(char *s)
{
	int	len;
	int	type;

	len = 0;
	type = get_type(*s);
	while (*s != '\0')
	{
		if (get_type(*s) == type)
			len++;
		else
			break;
		s++;
	}
	return (len);
}

char				**strsplit(char const *s)
{
	int			word_countx;
	char		**str;
	int			i;

	if (!s)
		return (NULL);
	word_countx = word_count((char *)s);
	str = (char**)malloc(sizeof(char*) * word_countx + 1);
	i = 0;
	if (!str)
		return (NULL);
	while (word_countx-- && *s)
	{
		str[i] = ft_strsub((char *)s, 0, word_len((char*)s));
		if (!str[i])
			return (NULL);
		s = s + word_len((char *)s);
		i++;
	}
	str[i] = 0;
	return (str);
}