/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 21:51:07 by macos             #+#    #+#             */
/*   Updated: 2020/11/29 15:12:42 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static size_t get_list_size(t_lexer *tokenz)
{
    size_t size;

    size = -1;
    if (tokenz)
    {
        size++;
        while (tokenz)
        {
            if (tokenz->data)
                size++;
            tokenz = tokenz->next;
        }
    }
    return (size);
}

static t_lexer      *move_list(t_lexer *tokenz)
{
    size_t tokenz_size;
    t_lexer *ret;
    t_lexer *cur;

    tokenz_size = get_list_size(tokenz);
    cur = tokenz;
    while (cur != NULL && (cur->coor.node_index < tokenz_size))
    {
        if (cur->type != SEP && cur->type != PIPE)
            cur = cur->next;
        else
            break ;       
    }
    if (cur->type == SEP || cur->type == PIPE)
        return (cur);
    else
    {
        ft_putendl_fd("I ENTER ELSE CONDITION IN {move_list fun} ret = NULL", 1);
        ret = NULL;
    }
    return (ret);
}

static void fill_redirections(t_redir **node, t_lexer *token)
{
    t_redir *cur;
    t_redir *new;

    if (node)
    {
        if(!(new = (t_redir*)ft_memalloc(sizeof(t_redir))))
            return ;
        if (token->type == L_REDIR)
            new->lfd = token->data;
        else if (token->type == R_REDIR)
            new->rfd = token->data;
        else
            new->sym = token->data;
        new->next = NULL;
        if (*node == NULL)
        {
            *node = new;
            return ;
        }
        cur = *node;
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = new;
        return ;
    }
}

size_t  get_arr_size_tokenz(t_lexer *token)
{
    size_t size;

    size = -1;
    if (token)
    {
        size++;
        while (token != NULL)
        {
            if (token->type != PIPE_SYM && token->type != METACHAR && token->type != SEP)
                size++;
            else
                break ;
            token = token->next;
        }
    }
    return (size);
}


char    **fill_node(t_lexer *token, t_redir **redirections, t_env **env)
{
    int i;
    char **ret;
    t_redir *redir;
    size_t ret_size;

    ret = NULL;
    if (token && env && redirections)
    {
        ret_size = get_arr_size_tokenz(token);
        if (!(ret = (char**)ft_memalloc(sizeof(char*) * (ret_size + 1))))
            return (NULL);
        i = 0;
        while (token != NULL && token->coor.node_index < ret_size)
        {
            if (token->type == WORD || token->type == DQUOT ||
                 token->type == SQUOT || token->type == EXPANSION) // 1
            {
                if (token->type != DQUOT)
                    ret[i] = token->data;
                else
                {
                    ret[i] = expanded(env, token->data);
                    //ft_strdel(&(token->data));
                }
            }
            else if (token->type == AGGR_SYM || 
                token->type == L_REDIR || token->type == R_REDIR) // 1
                fill_redirections(redirections, token);
            else // PIPE AND SEP
                break ;
            i++;
            token = token->next;
        }
        ret[i] = NULL;
    }
    return (ret);
}

int    parse_commands(t_miniast **head, t_lexer *tokenz, t_env **env)
{
    size_t tokenz_size;
    char **cmd;
    t_miniast *data;
    t_redir *redirections;

    cmd = NULL;
    tokenz_size = get_list_size(tokenz);
    while (tokenz && tokenz->coor.node_index < tokenz_size)
    {
        redirections = NULL;
        if ((*head) == NULL && env && tokenz->type != SEP && tokenz->type != PIPE_SYM)
        {
            if (!(data = (t_miniast*)ft_memalloc(sizeof(t_miniast))))
                return (-1);
            data->pipe = NULL;
            data->sep = NULL;
            data->redirection = NULL;    
            if (!(cmd = fill_node(tokenz, &redirections, env))) // fill commands and redirections
                return (-2);
            //if (redirections)
                //check_redirection: sym and right fd existant
            data->cmd = cmd;
            data->redirection = redirections;
            *head = data;
        }
        else
        {
            if (tokenz->type == PIPE_SYM)
            {
                //check_PIPE -> t_lexer *tokenz <-
                parse_commands(&(*head)->pipe, tokenz, env);
            }
            else if (tokenz->type == SEP)
                parse_commands(&(*head)->sep, tokenz, env);
        }
        tokenz = move_list(tokenz);
    }
    return (1);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

size_t calc_arr_size(t_lexer *token, int *next_type)
{
    size_t size;

    size = 0;
    *next_type = 0;
    if (!token)
        return (-1);
    while (token)
    {
        if (token->type != METACHAR && token->type != PIPE_SYM)
            size = size + 1;
        else
        {
            *next_type = token->type;
            break ;
        }
        token = token->next;
    }
    return (size);
}

t_miniast   *fill_miniast(t_lexer **head, t_miniast *pipe, t_type type)
{
    size_t cmd_size;
    t_miniast *ast;
    t_miniast **adrs;
    t_lexer *tokenz;
    t_redir *redirection;
    int next_type;
    int i;
    char *tmp;
    int node_idx = 0; //calc biggest_node
    

    redirection = NULL;
    ast = NULL;
    adrs = &ast;
    cmd_size = 0;
    if (head && *head)
    {
        tokenz = *head;
        while (node_idx--)
        {
            //Go to a specific node by it's index:
            //
            tmp = NULL;
            if ((cmd_size = calc_arr_size(tokenz, &next_type)) == -1)
                break ;
            (*adrs) = (t_miniast*)ft_memalloc(sizeof(t_miniast));
            (*adrs)->cmd = (char**)ft_memalloc(sizeof(char*)*(cmd_size + 1));
            // stock cmd until metachar and pipe 
            // {cmd_size, tokenz, head, adrs}
            i = 0;
            while (tokenz && i < cmd_size) 
            {
                if (tokenz->type == DQUOT && ft_is_expansion(tokenz->data))
                {
                    tmp = expanded(head, tokenz->data);
                    if (tmp)
                        (*adrs)->cmd[i] = ft_strdup(tmp);
                }
                // else if (ft_is_tilde(tokenz->data))
                // {
                    
                // }
                else
                    (*adrs)->cmd[i] = ft_strdup(tokenz->data);
                i++;
                tokenz = tokenz->next;
            }
            // if (next_type == PIPE_SYM)
            //     adrs = fill_miniast(head, &(*adrs)->pipe, PIPE_SYM);
            // else if (next_type == METACHAR)
            //     adrs = fill_miniast(head, &(*adrs)->sep, METACHAR);
            // else if (next_type == L_REDIR || next_type == AGGR_SYM || next_type == R_REDIR)
            // {
            //     ft_redirection(head, tokenz->type);
            // }
            // else
            //     break ;
            // if (tokenz->type == METACHAR)
            // {
            //     tokenz = tokenz->next;
            //     continue ;
            // }
            // else if (tokenz->type == WORD)
            // {
                
            // } 
            // tokenz = tokenz->next;
            tokenz = *head;
            node_idx++;
        }
        
    }
    return (ast);
}