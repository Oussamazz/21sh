/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 21:51:07 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/26 12:11:53 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_lexer      *move_list(t_lexer *tokenz, size_t alltokenzsize) //  ls -la dirlist eror | cat -e | wc -l |
{
    t_lexer *cur;

    if (!tokenz->next)
        return (NULL);
    cur = tokenz;
    while (cur != NULL && (cur->coor.node_index <= alltokenzsize))
    {
        if (cur->type != SEP && cur->type != PIPE_SYM)
            cur = cur->next;
        else
            break ;
    }
    if (cur && (cur->type == SEP || cur->type == PIPE_SYM))
        return (cur);
    return (NULL);
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
            new->lfd = ft_strdup(token->data);
        else if (token->type == R_REDIR)
            new->rfd = ft_strdup(token->data);
        else
            new->sym = ft_strdup(token->data);
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


char    **fill_node(t_lexer *token, t_redir **redirections, t_env **env, size_t alltoken_size)
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
        while (token != NULL && token->coor.node_index <= alltoken_size)
        {
            if (token->type == WORD || token->type == DQUOT ||
                 token->type == SQUOT || token->type == EXPANSION)
            {
                if (token->type != DQUOT)
                {
                    if (!token->coor.no_space)
                        ret[i] = ft_strdup(token->data);
                    else
                    {
                        if (token->next && token->next->data) // $HOME $TERM $PWD
                        {
                            ret[i] = ft_strjoin(token->data, token->next->data);
                            token = token->next;
                        }
                    }
                }
                else
                {
                    ret[i] = expanded(env, token->data);
                    ft_strdel(&(token->data));
                }
            }
            else if (token->type == AGGR_SYM || 
                token->type == L_REDIR || token->type == R_REDIR)
            {
                fill_redirections(redirections, token);
                i--;
            }
            else if (token->type == SEP || token->type == PIPE_SYM)
                break ;
            token = token->next;
            i++;
        }
        ret[i] = NULL;
    }
    return (ret);
}

int    parse_commands(t_miniast **head, t_lexer *tokenz, t_env **env)
{
    static size_t AlltokenzSize = 0;
    size_t tokenz_size;
    char **cmd;
    t_miniast *data;
    t_redir *redirections;

    cmd = NULL;
    if (!AlltokenzSize)
        AlltokenzSize = get_list_size(tokenz);
    while (tokenz && tokenz->coor.node_index <= AlltokenzSize)
    {
        redirections = NULL;
        if ((*head) == NULL && env && tokenz && tokenz->data) // ls -la;
        {
            if (!(data = (t_miniast*)ft_memalloc(sizeof(t_miniast))))
                return (-1);
            data->pipe = NULL;
            data->sep = NULL;
            data->cmd = NULL;
            data->redirection = NULL;    
            if (!(data->cmd = fill_node(tokenz, &(data->redirection), env, AlltokenzSize)))
                    return (-2);
            *head = data;
        }
        else
        {
            if (tokenz->type == PIPE_SYM && tokenz->next)
                parse_commands(&(*head)->pipe, tokenz->next, env);
            else if (tokenz->type == SEP && tokenz->next)
                parse_commands(&(*head)->sep, tokenz->next, env);
        }
        tokenz = move_list(tokenz, AlltokenzSize);
    }
    AlltokenzSize = 0;
    return (1);
}