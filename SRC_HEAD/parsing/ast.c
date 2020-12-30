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

static t_lexer      *move_list(t_lexer *tokenz, int alltokenzsize) //  ls -la dirlist eror | cat -e | wc -l |
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

static void fill_cmd(char **ret, t_lexer *token, int *i, t_env **env)
{
    char *tmp;

    if (token->type != DQUOT)
        ret[*i] = ft_strdup(token->data);
    else
    {
        ret[*i] = expanded(env, token->data);
        ft_strdel(&(token->data));
    }
    if (token->coor.no_space)
    {
        if (token->next && token->next->data &&
            (token->next->type == DQUOT || token->next->type == SQUOT || token->type == EXPANSION))
        {
            tmp = token->next->data;
            token->next->data = ft_strjoin(ret[*i], tmp);
            free(ret[*i]);
            *i -= 1;
            free(tmp);
        }
    }
}

int fill_cmd_redir(t_lexer *token, int *i, t_redir **redirections)
{
    if (token->type == AGGR_SYM ||
        token->type == L_REDIR || token->type == R_REDIR)
    {
        fill_redirections(redirections, token);
        *i -= 1;
    }
    else if (token->type == SEP || token->type == PIPE_SYM)
        return (1);
    return(0);
}

char    **fill_node(t_lexer *token, t_redir **redirections, t_env **env, int alltoken_size)
{
    int i;
    char **ret;
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
                    fill_cmd(ret, token, &i, env);
            else if (fill_cmd_redir(token, &i, redirections) == 1)
                break ;
            token = token->next;
            i++;
        }
        ret[i] = NULL;
    }
    return (ret);
}

static void parse_commands_sep_pipe(t_miniast **head, t_lexer *tokenz, t_env **env)
{
    if (tokenz->type == PIPE_SYM && tokenz->next)
        parse_commands(&(*head)->pipe, tokenz->next, env);
    else if (tokenz->type == SEP && tokenz->next)
        parse_commands(&(*head)->sep, tokenz->next, env);
}

int    parse_commands(t_miniast **head, t_lexer *tokenz, t_env **env)
{
    char        **cmd;
    t_miniast   *data;
    t_redir     *redirections;

    cmd = NULL;
    if (!g_alltokenzSize)
        g_alltokenzSize = get_list_size(tokenz);
    while (tokenz && tokenz->coor.node_index <= g_alltokenzSize)
    {
        redirections = NULL;
        if ((*head) == NULL && env && tokenz && tokenz->data)
        {
            if (!(data = (t_miniast*)ft_memalloc(sizeof(t_miniast))))
                return (-1);
            if (!(data->cmd = fill_node(tokenz, &(data->redirection), env, g_alltokenzSize)))
                return (-2);
            *head = data;
        }
        else
            parse_commands_sep_pipe(head, tokenz, env);
        tokenz = move_list(tokenz, g_alltokenzSize);
    }
    g_alltokenzSize = 0;
    return (1);
}
