/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_grammar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 17:32:00 by macos             #+#    #+#             */
/*   Updated: 2020/12/07 14:57:53 by macos            ###   ########.fr       */
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
    char *data;
    if (tokenz)
    {
        size_t tokenz_size = get_list_size(tokenz);
        while (tokenz && tokenz->coor.node_index <= tokenz_size)
        {
            data = tokenz->data;
            if (tokenz->type == AGGR_SYM && tokenz->next)
            {
                if (tokenz->next->type != R_REDIR && data[ft_strlen(data) - 1] != '-')
                {
                    error_message("Error 2020\n", 1);
                    return (-1);
                }
            }
            if (tokenz->type == L_REDIR)
            {
                if (tokenz->type == L_REDIR && (!tokenz->next))
                {
                        error_message("Error 2020 haha 2\n", 1);
                        return (-1);
                }
                if (tokenz->next && tokenz->next->type != AGGR_SYM)
                {
                    error_message("Error 2020 ahah 3\n", 1);
                }
            } 
            if (tokenz->type == PIPE_SYM && (!tokenz->next || tokenz->coor.node_index == 1))
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