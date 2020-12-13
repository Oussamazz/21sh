/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 17:13:38 by macos             #+#    #+#             */
/*   Updated: 2020/12/13 15:36:39 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static t_type ret_last_node_type(t_lexer **head)
{
    t_lexer *cur;

    if (head)
    {
        cur = *head;
        while (cur && cur->next)
            cur = cur->next;
        if (cur)
            return (cur->type);
    }
    return (0);
}

static void init_coord(t_pointt *cor)
{
    if (cor)
    {
        cor->node_index = 0;
        cor->node_addr = 0;
        cor->aggr_index = 1; // redirections
        cor->pipe_index = 1; // pipes     
    }

    return ;
}

int main(int ac,char **av, char **env)
{
    char *user;
    time_t now;
    static t_env *env_list;
    env_list = NULL;
    
    time(&now);
    stock_env(env, &env_list);
    user = NULL;
    if (ac > 1 && env_list)
    {
        if (!(user = get_value_expansion("USER", &env_list)))
            error_message("21sh: Error: USER NOT FOUND\n", 1);
        starting_message(av[1], &user, &now);
    }
    if(!(g_tty_name = ttyname(0)))
        return (1);
    source_sh(&env_list);
    return 0;
}

static void ft_print_redirections(t_redir   *node)
{
    if (node)
    {
        while (node)
        {
            if (node->lfd)
            {
                ft_putendl_fd("THIS IS LEFT_FD:", 1);
                ft_putendl_fd(node->lfd, 1);
            }
            else if (node->sym)
            {
                ft_putendl_fd("THIS IS SYM:", 1);
                ft_putendl_fd(node->sym, 1);
                
            }
            else if (node->rfd)
            {
                ft_putendl_fd("THIS IS RIGHT_FD:", 1);
                ft_putendl_fd(node->rfd, 1);
            }
            node = node->next;
        }
    }
}

void     print_btree(t_miniast *ast)
{
    if (!ast)
        return ;
    if (ast->cmd)
    {
        if (ast->redirection)
            ft_print_redirections(ast->redirection);
        print_arr(ast->cmd);
    }
    if (ast->pipe)
        ft_putendl_fd("i enter Pipe node", 1);
    if (ast->sep)
        ft_putendl_fd("i enter SEP node", 1);
    print_btree(ast->pipe);
    print_btree(ast->sep);
}

static void init_status(int *status)
{
    status[0] = 1;
    status[1] = 0;
}

void    source_sh(t_env **head)
{
    int status[2];
    char *buffer;
    t_lexer *tokenz;
    t_miniast *ast;
    t_pointt coord;

    buffer = NULL;
    tokenz = NULL;
    init_status(&status[0]);
    while (status[0] && g_tty_name)
    {
        ast = NULL;
        tokenz = NULL;
        init_coord(&coord);
        ft_prompte();
        if (!(buffer = ft_readline()))
            break ;
        tokenz = lexer(buffer, head, &coord);
        // print_list(tokenz = lexer(buffer, head, &coord));
        // ft_putendl_fd("\n_________________________", 1);
        //fflush(stdout); // not allowed
        status[1] = check_grammar_tokenz(tokenz);
        ast = NULL;
        //  ft_putendl_fd("THIS IS STATUS[1]:", 1);
        //  ft_putnbr_fd(status[1], 1);
        //  ft_putchar_fd('\n', 1);
        if (tokenz && head && status[1])
            status[1] = parse_commands(&ast, tokenz, head);
        // if (status[1] && ast)
        // {
        //     ft_putendl_fd("__________[Parse commands Completed BEGIN.]______________", 1);
        //     print_btree(ast);
        //     ft_putendl_fd("__________[Parse commands Completed END.]______________", 1);
        // }
        // else if (!status[1] && tokenz)
        //     ft_putendl_fd("__________[Parse commands Failed]______________", 1);
        // ft_putendl_fd("\n__________[EXECUTION]______________", 1);
        if (ft_strequ(buffer, "exit"))
            break ;
        else if (ft_strequ(buffer, "clear"))
            ft_putstr_fd("\e[1;1H\e[2J", 1);
        else if (status[1] && ast && head)
            status[0] = execute(ast, head, 1);
        //ft_free_tokenz(&tokenz);
        ft_free_tree(&ast);
        ft_strdel(&buffer);
    } 
}

t_lexer    *lexer(char *buf, t_env **env_list, t_pointt *coord)
{
    int i;
    int j;
    int position = 0;
    bool flag;
    char q;
    char **agg;
    char tmp[4096];
    char *temp;
    t_lexer *token_node;
    t_quote *quot;
    size_t size;

    token_node = NULL;
    i = 0;
    size_t buf_len = ft_strlen(buf);
    while (buf[i] != '\0' && i < buf_len)
    {
        while (buf[i] && is_blank(buf[i]))
            i++;
        if (buf[i] == ';' && buf[i + 1] == ';')
            return ((t_lexer*)err_ret("21sh: parse error near `;'\n", NULL));
        if (buf[i] == ';')
        {
            coord->pipe_index = 1;
            coord->aggr_index= 1;
            if (buf[i] == ';')
                i++;
            append_list(&token_node, ";", SEP, coord);
            continue ;
        }
        else if ((buf[i] == '$' || buf[i] == '~') && !(buf[i] == '$' && buf[i + 1] == '/')&& (buf[i] != buf[i + 1]) && (i == 0 || buf[i - 1] != '\\') && !is_quote(buf[i + 1]))
        { // Expansion
            if (buf[i] == '$' && (buf[i + 1] == '(' || buf[i + 1] == ')'))
                error_message("21sh: Unexpected token `( or )'\n", 1); // free
            i = i + expansion_parse(&token_node, buf + i, env_list, coord);
        }
        else if (buf[i] && ft_is_there(PIPE, buf[i]))
        {
            if ((position = parse_pipe(&token_node, buf + i - 1, coord)))
            {
                i = i + position;
                continue ;
            } // free
            return (NULL);
        }
        // else if (last_node_type(token_node) && !is_quote(buf[i]))
        // {
        //     temp = get_right_redir(buf + i);
        //     append_list_redi(&token_node, ft_strdup(temp), R_REDIR, coord);
        //     i = i + (int)calc_size_right_redir(buf + i);
        //     ft_strdel(&temp);
        // }
        else if (ft_is_there(AGG_REDI, buf[i]) && buf[i] && !check_quoting(&token_node, SQUOT, coord->aggr_index) && !check_quoting(&token_node, DQUOT, coord->aggr_index))
        {
            if (!buf[i + 1])
                error_message("21sh: Unexpected token. {2020}\n", 1);
            char *buf_dup = ft_strdup(buf + i);
            if (!(agg = split_redir(buf_dup, i)))
                return (NULL);
            i = i + redirerction_parse(&token_node, agg, coord, &i);
            g_agg_len = 0;
            ft_strdel(&buf_dup);
            continue ;
        }
        else if (((buf[i] == '\'' || buf[i] == '\"') || (buf[i] == '$' && is_quote(buf[i + 1]))) && (i == 0 || buf[i - 1] != '\\') && (i == 0 || is_blank(buf[i - 1])))
        {
            if (buf[i] == '$')
                i++;
            quot = quote_handling(buf + i + 1, buf[i], 1, env_list);
            if (!quot)
            {
                i++;
                continue ;   
            }
            if (quot->string && (buf[i] == '\'' || buf[i] == '\"') && ret_last_node_type(&token_node) == AGGR_SYM)
            {
                //ft_putendl_fd("append to r_redir", 1);
                append_list_redi(&token_node, ft_strdup(quot->string), R_REDIR, coord);
            }
            else if (buf[i] == '\'')
                append_list(&token_node, quot->string, SQUOT, coord);
            else
                append_list(&token_node, quot->string, DQUOT, coord);
            i += quot->size; // -1
            if (quot && quot->string && buf[i] != '\'' && buf[i] != '\"')
                ft_strdel(&(quot->string));//
        }
        else if (!ft_is_there(METACHARACTER, buf[i]) && buf[i] != '\n' && buf[i] != '\t' && buf[i] && !is_quote(buf[i]))
        {
            //here the prob // dquot Squot
            if (is_quote(q = valid_string_quot(buf + i)) || buf[i] == '\\') // before quote " or ' joining
            {
                quot = quote_handling(buf + i, q, 0, env_list);
                if (!quot)
                {
                    i++;
                    continue ;   
                }
                if (q == '\'')
                    append_list(&token_node, quot->string, SQUOT, coord);
                else
                    append_list(&token_node, quot->string, DQUOT, coord);
                i += quot->size;
                free(quot->string);
                free(quot);
            }
            else if (buf + i && *(buf + i)) // simple command
            {
                if (i > 1 && buf[i - 1] != '\\' && ft_isdigit(buf[i]) && ft_is_there(AGG_REDI, buf[i + 1]) && buf[i + 1] != '\0')
                {
                    temp = get_left_fd_(buf + i);
                    append_list_redi(&token_node, ft_strdup(temp), L_REDIR, coord);
                    i = i + ft_strlen(temp) - 1;
                    ft_strdel(&temp);
                }
                // if (i && check_command_redir_size(buf + i))
                // {
                //     temp = ft_strdup(buf + i);
                //     i = i + check_command_redir(&token_node, temp, coord) - 1;
                //     ft_strdel(&temp);
                //     continue ;
                // }
                else if (buf + i)
                {
                    j = 0;
                    int c = 0;
                    while (i < buf_len && !ft_is_there(METACHARACTER, buf[i + j]) && !ft_is_aggr(buf[i + j]) && buf[i + j] != '\n' && buf[i + j] != '\t' && buf[i + j] != '|' && buf[i + j])
                    {
                        if (buf[i + j] == '\\')
                        {
                            if (buf[i + j + 1] == '\\')
                            {
                                tmp[j] = buf[i + j];
                                j++;
                            }
                            i++;
                            continue ;
                        }
                        tmp[j] = buf[i + j];
                        j++;
                    }
                    tmp[j] = '\0';
                    append_list(&token_node, tmp, WORD, coord);
                    if (buf[i + j] == ';' || buf[i + j] == '<' || buf[i + j] == '>' || buf[i + j] == '|')
                        i--;
                    i = i + ft_strlen(tmp);
                    ft_strclr(tmp);
                }
            }
        }
        else if (ft_is_there(METACHARACTER, buf[i]) && !is_blank(buf[i]) && buf[i])
        {
            j = 0;
            while (ft_is_there(METACHARACTER, buf[i + j]) && !is_blank(buf[i + j]) && buf[i + j])
            {
                tmp[j] = buf[i + j];
                j++;
            }
            append_list(&token_node, tmp, METACHAR, coord);
            i = i + ft_strlen(tmp) - 1;
            ft_strclr(tmp);
        }
        i++;
    }
    return (token_node);
}

t_quote     *quote_handling(char *s, char quote, int start, t_env **env_list)
{
    t_quote *quot;
    t_quote *rec_quote;
    int i;
    int j;
    bool flag;
    char *part_string;
    char *tmp;

    i = 0;
    j = 0;
    flag = false;
    quot = (t_quote*)ft_memalloc(sizeof(t_quote));
    quot->string = ft_strnew(ft_strlen(s));
    if (s[i] == '\0')
    {
        //ft_putendl_fd("im at '0' quote handling and i'm entring comple", 1);
        return ((quot = quote_completion(&quot, quote, env_list)));
    }
    // if (s[i] == '\n')
    //     return (NULL);
    while (s[i] != '\0')
    {
        if (s[i] != quote && start && s[i] != '\\')
        {
            quot->string[j++] = s[i];
            flag = false;
        }
        else if (s[i] == '\\' && (i == 0 || s[i - 1] != '\\')) // quote != '\''
        {
            if (s[i + 1] != '\"' && s[i + 1] != '\'' && s[i + 1] != '$' && s[i + 1] != '\\' && start)
                quot->string[j++] = s[i];
            flag = true;
        }
        else if ((s[i] == '\'' || s[i] == '\"' || s[i] == quote) && flag == false)
        {
            if (s[i + 1] == '>' || s[i + 1] == '<')
                i = i + (ft_strchr(s + i, quote) - (s + i));
            else if (!is_blank(s[i + 1]) && !ft_is_there(METACHARACTER, s[i + 1]))
            {
                rec_quote = quote_handling(s + i + 1, s[i], !start, env_list);
                tmp = quot->string;
                quot->string = ft_strjoin(tmp, rec_quote->string);
                i = i + rec_quote->size + 1;
                free(rec_quote->string);
                free(tmp);
                free(rec_quote);
                break;
            }
            else if(s[i] != quote || !start)
                return (quote_completion(&quot, s[i], env_list));
            i++;
            break ;
        }
        else
        {
            quot->string[j++] = s[i];
            if (s[i + 1] == '>' || s[i + 1] == '<')
            {
                i++;
                break ;
            }
            flag = false;
        }
        if (!s[i + 1] && start)
            return (quote_completion(&quot, quote, env_list));
        i++;
    }
    quot->size = i;
    return (quot);
}
