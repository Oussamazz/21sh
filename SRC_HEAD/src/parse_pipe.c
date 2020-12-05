/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 16:29:56 by macos             #+#    #+#             */
/*   Updated: 2020/12/04 16:47:30 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int      parse_pipe(t_lexer **token_node, char *str, t_pointt *cor)
{
    char tmp[2];
    int i;
    int j;

    i = 0;
    j = 0;
    while (str[i]) // ls |  |  |
    {
        if (str[i] == '|' && str[i - 1] != '\\' && (is_blank(str[i + 1]) || !str[i + 1]))
        {
            tmp[j] = str[i];
            j++;
            i++;
            break ;
        }
        i++;
    }
    tmp[j] = '\0';
    append_list_pipe(token_node, ft_strdup(tmp), PIPE_SYM, cor);
    return (1);
}