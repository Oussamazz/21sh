/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/30 09:02:59 by macos             #+#    #+#             */
/*   Updated: 2020/11/30 10:10:39 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int     check_tokenz_grammar(t_lexer *tokenz)
{
    size_t size;
    t_lexer *cur;
    t_lexer *prev;
    t_lexer *next;
    
    if (tokenz)
    {
        cur = tokenz;
        prev = NULL;
        next = cur->next;
        size = get_list_size(tokenz);
        while (cur && next && cur->coor.node_index < size)
        {
            next = cur->next;
            if (cur->type == R_REDIR)
            {
                if (prev->type != AGGR_SYM)
                    error_message("SYNTAX ERROR 0", 1);
            }
            else if (cur->type == AGGR_SYM)
            {
                if ((prev->type == PIPE_SYM || prev->type == SEP) || next->type != R_REDIR)
                    error_message("SYNTAX ERROR 1", 1);
            }
            else if (cur->type == L_REDIR)
            {
                if (next->type != AGGR_SYM)
                    error_message("SYNTAX ERROR 2", 1);
            }
            prev = cur;
            cur = next;
        }
        if (cur->type == PIPE_SYM || (cur->type == AGGR_SYM && !ft_strequ(cur->data, "<<")))
            error_message("SYNTAX ERROR 3", 1);
        return (1);
    }
    return (-1);
}