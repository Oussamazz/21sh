/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   his.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 17:44:38 by macos             #+#    #+#             */
/*   Updated: 2020/12/23 04:41:18 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    add_to_his(char *buffer, t_his **g_his, int flag)
{
    t_his *last;
    t_his *ret;

    if (buffer && !*buffer)
        return ;
    last = *g_his;
	if (!(ret = (t_his*)ft_memalloc(sizeof(t_his))))
        return ;
    if (flag)
        ret->data = ft_strjoin("\n", buffer);
    else
        ret->data = ft_strdup(buffer);
    if (last == NULL)
    {
        *g_his = ret;
        return ;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = ret;
}

char    *join_all_bufs(t_his *his)
{
    char *ret;
    char *tmp;

    ret = NULL;   
    if (his)
    {
        ret = ft_strdup("");
        while (his)
        {
            tmp = ret;
            if (!(ret = ft_strjoin(ret, his->data)))
                return (NULL);
            ft_strdel(&tmp);
            his = his->next;
        }      
    }
    return (ret);
}

/*

    t_lexer *last;
    t_lexer *ret;

    last = *root;
    ret = (t_lexer*)ft_memalloc(sizeof(t_lexer));
    if (ret == NULL)
        return ;
    ret->data = ft_strdup(data);
    ret->type = type;
    if (type != METACHAR)
    {
        cor->node_index += 1;
        ret->coor.node_index = cor->node_index;
        ret->coor.no_space = cor->no_space;
    }
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

*/
