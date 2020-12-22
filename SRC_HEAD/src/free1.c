/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 21:17:08 by macos             #+#    #+#             */
/*   Updated: 2020/12/22 19:27:18 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    ft_free_his(t_his **g_his)
{
    t_his *cur;
    t_his *tmp;

    if (g_his)
    {
        cur = *g_his;
        while (cur)
        {
            tmp = cur;
            ft_strdel(&(cur->data));
            cur = cur->next;
            free(tmp);
        }
        *g_his = NULL;
    }
}

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

static void    free_list_redir(t_redir *redirections)
{
    t_redir *next;

    next = NULL;
    while (redirections)
    {
        if (redirections->lfd)
            ft_strdel(&redirections->lfd);
        if (redirections->sym)
            ft_strdel(&redirections->sym);
        if (redirections->rfd)
            ft_strdel(&redirections->rfd);
        next = redirections->next;
        if (redirections)
            free(redirections);
        redirections = next;
    }
    redirections = NULL;
    return ;
}

void    ft_free_tree(t_miniast **tree)
{
    if (!*tree)
        return ;
    ft_free_arr((*tree)->cmd);
    if ((*tree)->redirection)
        free_list_redir((*tree)->redirection);
    ft_free_tree(&(*tree)->pipe);
    ft_free_tree(&(*tree)->sep);
    ft_memdel((void**)tree);
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
       *head = NULL;
    }
    return ;
}