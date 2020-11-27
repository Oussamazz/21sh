/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 21:51:07 by macos             #+#    #+#             */
/*   Updated: 2020/11/27 20:34:56 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

size_t  get_arr_size_tokenz(t_lexer *token)
{
    size_t size;

    size = 0;
    while (token != NULL)
    {
        if (token->type != PIPE_SYM && token->type != METACHAR && token->type != SEP)
            size++;
        else
            break ;
        token = token->next;
    }
    return (size);
}


char    **get_command(t_lexer *token, t_miniast **data, t_env **env)
{
    int i;
    char **ret;
    size_t ret_size;

    ret = NULL;
    if (token)
    {
        ret_size = get_arr_size_tokenz(token);
        if (!(ret = (char**)ft_memalloc(sizeof(char*) * (ret_size + 1))))
            return (NULL);
        i = 0;
        while (token != NULL)
        {
            if (!(ret[i] = (char*)ft_memalloc(sizeof(char) * MAX_INDEX + 1)))
                return (NULL);
            if (token->type == WORD || token->type == DQUOT ||
                 token->type == SQUOT || token->type == EXPANSION)
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
                token->type == L_REDIR || token->type == R_REDIR)
            {
                
            }
            i++;
            token = token->next;
        }
        ret[i] = NULL;
    }
    return (ret);
}

int    parse_commands(t_miniast **head, t_lexer *tokenz, t_env **env)
{
    char **cmd;
    t_miniast *data;

    cmd = NULL;
    while (tokenz)
    {
        if ((*head) == NULL && env)
        {
            data = (t_miniast*)ft_memalloc(sizeof(t_miniast));
            if (!(cmd = get_command(tokenz, &data, env)))
                return (-1);
            data->cmd = cmd;
            data->pipe = NULL;
            data->sep = NULL;
            *head = data;
        }
        else if (tokenz->type == PIPE_SYM)
            parse_commands(&(*head)->pipe, tokenz, env);
        else if (tokenz->type == SEP)
            parse_commands(&(*head)->sep, tokenz, env);
        tokenz = tokenz->next;
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