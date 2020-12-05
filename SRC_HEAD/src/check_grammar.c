/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_grammar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 17:32:00 by macos             #+#    #+#             */
/*   Updated: 2020/12/04 23:48:50 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int print_error(t_type type)
{
    err_ret("bash: syntax error near unexpected token `", NULL);
    if (type == PIPE_SYM)
        ft_putendl_fd("|'", 2);
    else if (type == SEP)
        ft_putendl_fd(";'", 2);
    return (0);
}

static int     check_token_type(t_type type, t_lexer *tokenz, t_lexer *next, size_t tokenz_size)
{
    if (tokenz->type == type && next->coor.node_index <= tokenz_size
            && (next->type == type) ||
            next->type != WORD && next->type != SQUOT &&
             next->type != DQUOT && next->type != EXPANSION)
    {
        print_error(type);
        return (0);
    }
    return (1);
}

int     check_grammar_tokenz(t_lexer *tokenz)
{
    if (tokenz)
    {
        size_t tokenz_size = get_list_size(tokenz);
        while (tokenz && tokenz->coor.node_index <= tokenz_size)
        {
            if (tokenz->type == PIPE_SYM && !tokenz->next)
                return (print_error(PIPE_SYM));
            else if (tokenz->type == PIPE_SYM && tokenz->next)
                if (!(check_token_type(PIPE_SYM, tokenz, tokenz->next, tokenz_size)))
                    return (0);
            if (tokenz->type == SEP && tokenz->next)
                if (!(check_token_type(SEP, tokenz, tokenz->next, tokenz_size)))
                    return (0);
            tokenz = tokenz->next;
        }
        return (1);
    }
    return (0);
}