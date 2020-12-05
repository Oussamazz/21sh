/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 09:38:58 by macos             #+#    #+#             */
/*   Updated: 2020/12/05 16:50:21 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

size_t get_list_size(t_lexer *tokenz)
{
    size_t size;

    size = -1;
    if (tokenz)
    {
        size++;
        while (tokenz)
        {
            if (tokenz->data)
                size++;
            tokenz = tokenz->next;
        }
    }
    return (size);
}

size_t      ft_strlen_char2(char *s, char c, char c2)
{
    size_t len;

    len = 0;
    while (s[len] != c && s[len] != c2 && !ft_is_there(AGG_REDI, s[len]) && s[len] != '|')
        len++;
    if (s[len] == c2 || s[len] == '|' || ft_is_there(AGG_REDI, s[len]))
        g_agg_len--;
    return (len);
}