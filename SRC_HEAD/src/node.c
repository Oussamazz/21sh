/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/12 15:37:17 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/27 01:15:38 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    deleteNode(t_env **head_ref, char *env_name)
{
    t_env *temp;
    t_env *prev;

    temp = *head_ref;
    if (temp != NULL && ft_strequ(temp->env_var_name, env_name))
    {
        *head_ref = temp->next;
        ft_strdel(&temp->env_var_name);
        ft_strdel(&temp->env_var_value);
        free(temp);
        return ;
    }
    while (temp != NULL && ft_strcmp(temp->env_var_name, env_name))
    {
        prev = temp; 
        temp = temp->next; 
    }
    if (temp == NULL)
        return ; 
    prev->next = temp->next; 
    ft_strdel(&temp->env_var_name);
    ft_strdel(&temp->env_var_value);
    free(temp);
}

void    append_list(t_lexer **root, char *data, t_type type, t_pointt *cor)
{
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
}

void    append_list_redi(t_lexer **root, char *data, t_type type, t_pointt *cor)
{
    t_lexer *last;
    t_lexer *ret;

    last = *root;
    if (!(ret = (t_lexer*)ft_memalloc(sizeof(t_lexer))))
        return ;
    ret->data = data;
    ret->type = type;
    ret->coor.aggr_index = cor->aggr_index;
    cor->aggr_index += 1;
    cor->node_index += 1;
    ret->coor.node_index = cor->node_index;
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

void    append_list_pipe(t_lexer **root, char *data, t_type type, t_pointt *cor)
{
    t_lexer *last;
    t_lexer *ret;

    last = *root;
    if (!(ret = (t_lexer*)ft_memalloc(sizeof(t_lexer))))
        return ;
    ret->data = data;
    ret->type = type;
    ret->coor.pipe_index = cor->pipe_index;
    cor->pipe_index += 1;
    cor->node_index += 1;
    ret->coor.node_index = cor->node_index;
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
    int i;

    i = 0;
     while (token_list != NULL && i < token_list->coor.node_index)
     {
        ft_putnbr_fd(token_list->type, 1);
        ft_putchar_fd(' ', 1);
        if (token_list->data)
            ft_putstr_fd(token_list->data, 1);
        ft_putstr_fd("...........|| aggr_index = ", 1);
        ft_putnbr_fd(token_list->coor.aggr_index, 1);
        ft_putstr_fd(" || pipe_index = ", 1);
        ft_putnbr_fd(token_list->coor.pipe_index, 1);
        ft_putstr_fd(" || node_index = ", 1);
        ft_putnbr_fd(token_list->coor.node_index, 1);
        ft_putnbr_fd(token_list->coor.no_space, 1);
        ft_putchar_fd('\n', 1);
        token_list = token_list->next;
        i++;
     }
}

void    print_list2(t_redir  *redirections)
{
    if (redirections)
    {
        while (redirections)
        {
            if (redirections->lfd)
            {
                ft_putstr_fd("THIS IS L_FD: ", 1);
                ft_putendl_fd(redirections->lfd, 1);
            }
            else if (redirections->sym)
            {
                ft_putstr_fd("THIS IS SYM: ", 1);
                ft_putendl_fd(redirections->sym, 1);
            }
            else if (redirections->rfd)
            {
                ft_putstr_fd("THIS IS R_FD: ", 1);
                ft_putendl_fd(redirections->rfd, 1);
            }
            redirections = redirections->next;
        }
    }
    return ;
}