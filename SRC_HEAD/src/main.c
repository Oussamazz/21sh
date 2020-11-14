/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/08 03:53:10 by macos             #+#    #+#             */
/*   Updated: 2020/11/13 22:24:33 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"


int main(int ac,char **av, char **env)
{
    t_env *env_list;

    env_list = NULL;
    stock_env(env, &env_list);
    source_sh(&env_list);
    return 0;
}

void    source_sh(t_env **head)
{
    char *buffer;
    t_lexer *tokenz;

    buffer = NULL;
    tokenz = NULL;
    while (1)
    {
        buffer = readline("$> ");
        print_list((tokenz = lexer(buffer, head)));
        if (ft_strequ(buffer, "exit"))
            exit(0);
        else if (ft_strequ(buffer, "env"))
            print_env_list(head);
        else if (ft_strequ(buffer, "clear"))
            ft_putstr_fd("\e[1;1H\e[2J", 1);
        free(buffer);
    } 
}

// static int     check_buf(char *buf, int i, t_lexer **token_node)
// {
//     int j;
//     int counter;
//     char tmp[4096];
    
//         if (buf[i - 1] && !is_blank(buf[i - 1]))
//         {
//             j = 0;
//             counter = i - 1;
//             while (ft_isalpha(buf[counter]) && counter >= 0 && !is_blank(buf[counter]))
//             {
//                 ft_putchar(buf[counter]);
//                 tmp[j] = buf[counter];
//                 counter--;
//                 j++;
//             }
//             tmp[j] = '\0';
//             append_list(token_node, tmp, L_REDIR);
//             return(1);
//         }
//     return 0;
// }

t_lexer    *lexer(char *buf, t_env **env_list)
{
    int i;
    int j;
    bool flag;
    char q;
    t_pointt coord;
    coord.aggr_index = 1;
    coord.pipe_index = 1;
    char **agg;
    char tmp[4096];
    char *temp;
    t_lexer *token_node;
    t_quote *quot;
    size_t size;

    token_node = NULL;
    i = 0;
    
    while (buf[i] != '\0')
    {
        while (buf[i] && is_blank(buf[i]))
            i++;
        if (buf[i] == ';')
        {
            while (buf[i] == ';' || is_blank(buf[i]))
                i++;
            coord.pipe_index = 1;
            coord.aggr_index= 1;
            append_list(&token_node, ";", SEP);
            continue ;
        }
        else if (buf[i] == '$' && buf[i + 1] && ft_isalnum(buf[i + 1])) // Expansion
        {
            i = i + expansion_parse(&token_node, buf + i, env_list);
        }
        else if (buf[i] && ft_is_there(PIPE, buf[i]))
        {
            i = i + parse_pipe(&token_node, buf + i - 1, &coord);
        }
        else if (last_node_type(&token_node) == AGGR_SYM)
        {
            append_list_redi(&token_node, ft_strdup((temp = get_right_redir(buf + i))), R_REDIR, &coord);
            i = i + (int)calc_size_right_redir(buf + i);
            ft_strdel(&temp);
        }
        else if (ft_is_there(AGG_REDI, buf[i]) && buf[i] && !check_quoting(&token_node, SQUOT, coord.aggr_index) && !check_quoting(&token_node, DQUOT, coord.aggr_index))
        {
            char *buf_dup = ft_strdup(buf + i);
            if (!(agg = split_redir(buf_dup, i)))
                continue ;
            i = i + redirerction_parse(&token_node, agg, &coord, &i);
            ft_strdel(&buf_dup);
        }
        else if ((buf[i] == '\'' || buf[i] == '\"') && (i == 0 || buf[i - 1] != '\\') && (i == 0 || is_blank(buf[i - 1])) && buf[i+1])
        {
            quot = quote_handling(buf + i + 1, buf[i], 1);
            if (quot->string && (buf[i] == '\'' || buf[i] == '\"') && last_node_type(&token_node) == AGGR_SYM)
            {
                ft_putendl_fd(quot->string, 1);
                append_list_redi(&token_node, quot->string, R_REDIR, &coord);
            }
             if (buf[i] == '\'')
                append_list(&token_node, quot->string, SQUOT);
            else
                append_list(&token_node, quot->string, DQUOT);
            i += quot->size; // -1
            free(quot->string);
            free(quot);
        }
        else if (!ft_is_there(METACHARACTER, buf[i]) && buf[i] != '\n' && buf[i] != '\t' && buf[i] &&  buf[i] != '\'' && buf[i] != '\"')
        {
            //here the prob
            if ((q = valid_string_quot(buf + i)) != '\0' || buf[i] == '\\')
            {
                quot = quote_handling(buf + i, '\"', 0);
                if (q == '\'')
                    append_list(&token_node, quot->string, SQUOT);
                else
                    append_list(&token_node, quot->string, DQUOT);
                i += quot->size;
                free(quot->string);
                free(quot);
            }
            else if (buf + i && *(buf + i))
            {
                if (i && check_command_redir_size(buf + i))
                {
                    temp = ft_strdup(buf + i);
                    i = i + check_command_redir(&token_node, temp, &coord) - 1;
                    ft_strdel(&temp);
                    continue ;
                }
                else
                {
                    j = 0;      
                    while (!ft_is_there(METACHARACTER, buf[i + j]) && buf[i + j] != '<' && buf[i + j] != '>' && buf[i + j] != '\n' && buf[i + j] != '\t' && buf[i + j])
                    {
                        tmp[j] = buf[i + j];
                        j++;
                    }
                    tmp[j] = '\0';
                    append_list(&token_node, tmp, WORD);
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
            append_list(&token_node, tmp, METACHAR);
            i = i + ft_strlen(tmp) - 1;
            ft_strclr(tmp);
        }
        i++;
    }
    return token_node;
}

t_quote     *quote_handling(char *s, char quote, int start)
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
    while (s[i] != '\0')
    {
        if (s[i] != quote && start && s[i] != '\\')
        {
            quot->string[j++] = s[i];
            flag = false;
        }
        else if (s[i] == '\\' && (i == 0 || s[i - 1] != '\\') && quote != '\'')
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
                    rec_quote = quote_handling(s + i + 1, '\"', !start);
                else
                    rec_quote = quote_handling(s + i + 1, s[i], !start);
                tmp = quot->string;
                quot->string = ft_strjoin(tmp, rec_quote->string);
                i = i + rec_quote->size + 1;
                free(rec_quote->string);
                free(tmp);
                free(rec_quote);
                break;
            }
             // ?quote> prompt must be handled
            else if(s[i] != quote || !start)
            {
                ft_putstr("\nERROR\n");
                //quote_completion();
                //quot->string[j++] = s[i];
            }
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
        {
            ft_putstr("\nERROR\n");
            //quote_completion(&quot, quote);
        }
        i++;
    }
    quot->size = i;
    return (quot);
}
