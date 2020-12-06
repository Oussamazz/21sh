/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 21:17:08 by macos             #+#    #+#             */
/*   Updated: 2020/12/05 17:08:30 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    free_quot(t_quote **data)
{
    t_quote *my_quote;

    if (data && *data)
    {
        my_quote = *data;
        ft_strdel(&(my_quote->string));
        free(my_quote);
        *data = NULL;
    }
}

void    ft_free_tree(t_miniast **tree)
{
    if (!*tree)
        return ;
    ft_free_arr((*tree)->cmd);
    ft_free_tree(&(*tree)->pipe);
    ft_free_tree(&(*tree)->sep);
    //free_list((*tree)->redirections);
    free(*tree);
}

void    ft_free_tokenz(t_lexer **head)
{
    t_lexer *cur;
    t_lexer *tmp;

    if (head && *head)
    {
        cur = *head;
        while (cur)
        {
            tmp = cur;
            if (cur->data)
                ft_strdel(&(cur->data));
            cur = cur->next;
            free(tmp);
        }
        if (cur)
            free(cur);
        cur = NULL;
    }
    return ;
}