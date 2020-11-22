/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 21:17:08 by macos             #+#    #+#             */
/*   Updated: 2020/11/21 21:22:14 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    ft_free_tokenz(t_lexer **head)
{
    t_lexer *cur;
    t_lexer *tmp;

    if (head || *head)
    {
        cur = *head;
        while (cur)
        {
            tmp = cur;
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