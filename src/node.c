/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/12 15:37:17 by macos             #+#    #+#             */
/*   Updated: 2020/07/12 17:01:59 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    append_list(t_lexer **root, char *data, t_type type)
{
    t_lexer *last;
    t_lexer *ret;

    last = *root;

    ret = (t_lexer*)ft_memalloc(sizeof(t_lexer));
    if (ret == NULL)
        return ;
    ret->data = ft_strdup(data);
    ret->type = type;
    ret->next = NULL;
    if (last == NULL)
    {
        *root = ret;
        return ;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = ret;
    return ;
}

void    print_list(t_lexer *token_list)
{
    while (token_list != NULL)
    {
        ft_putnbr_wspace(token_list->type);
        ft_putendl(token_list->data);
        token_list = token_list->next;
    }
}