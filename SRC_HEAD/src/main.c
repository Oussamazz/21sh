/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 03:53:10 by macos             #+#    #+#             */
/*   Updated: 2020/12/01 13:41:58 by macos            ###   ########.fr       */
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
    t_env *env_list;
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
    source_sh(&env_list);
    return 0;
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
    status[0] = 1;
    while (status[0])
    {
        status[1] = 0;
        init_coord(&coord);
        ft_prompte();
        buffer = ft_readline();
        print_list((tokenz = lexer(buffer, head, &coord)));
        //fflush(stdout); // not allowed
        //status[1] = check_tokenz_grammar(tokenz);
        //ast = NULL;
        // if (tokenz && head && status[1])
        //     status[1] = parse_commands(&ast, tokenz, head);
        //ast <-
        //status = execute(&tokenz, head);
        if (ft_strequ(buffer, "exit"))
            exit(0);
        else if (ft_strequ(buffer, "env"))
            print_env_list(head);
        else if (ft_strequ(buffer, "clear"))
            ft_putstr_fd("\e[1;1H\e[2J", 1);
         //not allowed!!
        ft_free_tokenz(&tokenz);
        ft_strdel(&buffer);
    } 
}

t_lexer    *lexer(char *buf, t_env **env_list, t_pointt *coord)
{
    int i;
    int j;
    t_type node_type = 0;
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
            error_message("21sh: parse error near `;'\n", 1);
        if (buf[i] == ';')
        {
            coord->pipe_index = 1;
            coord->aggr_index= 1;
            append_list(&token_node, ";", SEP, coord);
            i++;
            continue ;
        }
        else if ((buf[i] == '$' || buf[i] == '~') && !(buf[i] == '$' && buf[i + 1] == '/')&& (buf[i] != buf[i + 1]) && (i == 0 || buf[i - 1] != '\\') && !is_quote(buf[i + 1]))
        { // Expansion
            if (buf[i] == '$' && (buf[i + 1] == '(' || buf[i + 1] == ')'))
                error_message("21sh: Unexpected token `( or )'\n", 1);
            i = i + expansion_parse(&token_node, buf + i, env_list, coord);
        }
        else if (buf[i] && ft_is_there(PIPE, buf[i]))
            i = i + parse_pipe(&token_node, buf + i - 1, coord);
        // else if (last_node_type(token_node) && !is_quote(buf[i]))
        // {
        //     temp = get_right_redir(buf + i);
        //     append_list_redi(&token_node, ft_strdup(temp), R_REDIR, coord);
        //     i = i + (int)calc_size_right_redir(buf + i);
        //     ft_strdel(&temp);
        // }
        else if (ft_is_there(AGG_REDI, buf[i]) && buf[i] && !check_quoting(&token_node, SQUOT, coord->aggr_index) && !check_quoting(&token_node, DQUOT, coord->aggr_index))
        {
            char *buf_dup = ft_strdup(buf + i);
            if (!(agg = split_redir(buf_dup, i)))
                continue ;
            i = i + redirerction_parse(&token_node, agg, coord, &i);
            ft_strdel(&buf_dup);
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
                ft_putendl_fd("append to r_redir", 1);
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
                if (i && check_command_redir_size(buf + i))
                {
                    temp = ft_strdup(buf + i);
                    i = i + check_command_redir(&token_node, temp, coord) - 1;
                    ft_strdel(&temp);
                    continue ;
                }
                else
                {
                    j = 0;
                    int c = 0;
                    while (i < buf_len && !ft_is_there(METACHARACTER, buf[i + j]) && !ft_is_aggr(buf[i + j]) && buf[i + j] != '\n' && buf[i + j] != '\t' && buf[i + j])
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
                    if (buf[i + j] == ';' || buf[i + j] == '<' || buf[i + j] == '>')
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
            tmp[j] = '\0';
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
        ft_putendl_fd("im at '0' quote handling and i'm entring comple", 1);
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
                if (start)
                    rec_quote = quote_handling(s + i + 1, s[i], !start, env_list);
                else
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
                return (quote_completion(&quot, quote, env_list));
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
    //  printf("1000---->%s\n", quot->string);
    // printf("size is %zu\n", ft_strlen(quot->string));
    quot->size = i;
    // printf("size is %zu\n", quot->size);
    return (quot);
}
