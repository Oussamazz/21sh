/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 17:13:38 by macos             #+#    #+#             */
/*   Updated: 2020/12/25 19:09:37 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void free_env_list(t_env **head)
{
	t_env *cur;
	t_env *tmp;

	cur = *head;
	while (cur)
	{
		tmp = cur->next;
		ft_strdel(&cur->env_var_name);
		ft_strdel(&cur->env_var_value);
		free(cur);
		cur = tmp;
	}
}

t_type ret_last_node_type(t_lexer **head)
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
		cor->no_space = 0;
		cor->aggr_index = 1; // redirections
		cor->pipe_index = 1; // pipes
	}
	return;
}

int main(int ac, char **av, char **env)
{
	char *user;
	time_t now;
	t_env *env_list;

	env_list = NULL;
	stock_env(env, &env_list); // ft-getenv
	time(&now);
	user = NULL;
	if (ac > 1 && env_list)
	{
		if (!(user = get_value_expansion("USER", &env_list)))
			error_message("21sh: Error: USER NOT FOUND\n", 1); // free
		starting_message(av, &user, &now);
	}
	if (!(g_tty_name = ttyname(0)))
		return (1); // free
	source_sh(&env_list);
	env_list = NULL;
	if (g_tty_name)
		free(g_tty_name);
	return (0);
}

static void ft_print_redirections(t_redir *node)
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

void print_btree(t_miniast *ast)
{
	if (!ast)
		return;
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

void ft_ctrlc(int sig_no)
{
	sig_no = 0;
	while (wait(NULL) > 0)
		;
	ft_putchar_fd('\n', 1);
	ft_prompte();
	prompt_flag = 1;
}

void print_his(t_his *g_his)
{
	while (g_his)
	{
		ft_putstr_fd(g_his->data, 1);
		g_his = g_his->next;
	}
}

void source_sh(t_env **head)
{
	t_mystruct v;

	signal(SIGINT, ft_ctrlc);
	ft_bzero(&v,sizeof(t_mystruct));
	v.status[0] = 1;
	while (v.status[0] && g_tty_name)
	{
		init_coord(&v.coord);
		ft_prompte();
		if (!(v.str = ft_readline(0)))
			break;
		add_to_his(v.str, &g_his, 0);
		v.tokenz = lexer(v.str, head, &v.coord);
		v.status[1] = check_grammar_tokenz(v.tokenz);
		if (v.tokenz && head && v.status[1] > 0)
			v.status[1] = parse_commands(&v.ast, v.tokenz, head);
		if (v.str[0] != '\0')
			add_to_history(v.tmp = join_all_bufs(g_his));
		if (v.status[1] > 0 && v.ast && head && v.ast->cmd)
			v.status[0] = execute(v.ast, head);
		free_vars(&v,(int[]){F_TMP,F_TOKENZ,F_AST,F_STR,F_G_HIS},5);
	}
}
/*
static int func_comp1(char *buf, t_lexer **token_node, t_env **env_list, t_pointt *coord)
{
	int i;
	int position;

	position = 0;
	i = 0;
	if (buf[i] == ';')
	{
		if ((position = sep_function(buf + i, token_node, coord)) > 0)
			i = i + position - 1;
		else
		{
			ft_free_tokenz(token_node);
			return (-1);
		}
	}
	else if ((buf[i] == '$' || buf[i] == '~') && !(buf[i] == '$' && buf[i + 1] == '/') && (buf[i] != buf[i + 1]) && (i == 0 || buf[i - 1] != '\\') && !is_quote(buf[i + 1])) //-> int expansion_function(char *, t_lexer **, t_pointt *, t_env **)
	{
		if ((position = expansion_function(buf, token_node, coord, env_list)) > 0)
			i = i + position - 1;
		else
		{
			ft_free_tokenz(token_node);
			return (-1);
		}
	}
	return (i);
}

static int fun_comp2(char *buf, t_lexer **token_node, t_pointt *coord)
{
	int i;
	int position;

	position = 0;
	i = 0;
	if (buf[i] && ft_is_there(PIPE, buf[i])) //-> int    pipe_function (char *, t_pointt *, t_lexer **)
	{
		if ((position = parse_pipe(token_node, buf + i - 1, coord)))
			i = i + position - 1;
		else
			return (-1);
	}
	else if (ft_is_there(AGG_REDI, buf[i]) && buf[i]) //-> void  aggr_functioin(char *, t_pointt *, t_lexer **, int *)            ||&& !check_quoting(&token_node, SQUOT, coord->aggr_index) && !check_quoting(&token_node, DQUOT, coord->aggr_index)
	{
		if (!*token_node)
			return (print_error_sym(AGGR_SYM));
		if ((position = aggr_function(buf, coord, token_node)) == -1)
			return (-1);
		else
			i += position;
	}
	return (i);
}

static int fun_comp3(char *buf, t_lexer **token_node, t_pointt *coord, size_t buf_len)
{
	int i;
	int position;
	char q;
	t_quote *quot;

	i = 0;
	position = 0;
	if ((buf[i] == '\'' || buf[i] == '\"') || (((buf[i] == '$' && is_quote(buf[i + 1]))) && (i == 0 || buf[i - 1] != '\\') && (i == 0 || buf[i - 1] == ';'))) //->      int     quote_function(char *buf, t_lexer **,t_pointt *, t_env **env_list)
	{
		position = quote_function(buf + i, token_node, coord);
		if (position < 0)
		{
			if (!g_clt_c)
				ft_putstr_c_str("21sh: unexpected EOF while looking for matching `", buf[i], "\'\n", 2);
			return (-1);
		}
		i += position;
	}
	else if (!ft_is_there(METACHARACTER, buf[i]) && buf[i] && !is_quote(buf[i])) // word
	{
		if (is_quote(q = valid_string_quot(buf + i)) || buf[i] == '\\') // before quote " or ' joining
		{
			if (!(quot = quote_handling(buf + i, q, 0))) //->    int     quote_handling_function(t_lexer **, t_quote *, char quote, t_pointt *)
				return (-1);
			i += quote_handling_function(token_node, quot, q, coord);
		}
		else if (buf + i && *(buf + i)) // simple command simple_word_function(char *,  t_lexer **, t_pointt *, size_t buf_len)
			i += simple_word_function(buf + i, token_node, coord, buf_len);
	}
	return (i);
}

t_lexer *lexer(char *buf, t_env **env_list, t_pointt *coord)
{
	t_mystruct v;
	// int i;
	// int j;
	// int position = 0;
	// char q;
	// t_lexer *token_node;
	// t_quote *quot;

	// token_node = NULL;
	// quot = NULL;
	// i = 0;
	
	ft_bzero(&v, sizeof(t_mystruct));
	size_t buf_len = ft_strlen(buf);
	while (buf[v.i] != '\0' && v.i < buf_len)
	{
		while (buf[v.i] && is_blank(buf[i]))
			v.i++;
		if ((buf[i] == ';' && buf[i + 1] == ';') || (buf[i] == ';' && !v.tokenz))
			return ((t_lexer *)err_ret("21sh: parse error near `;'\n", NULL));
		if (buf[i] == ';' || ((buf[i] == '$' || buf[i] == '~') && !(buf[i] == '$' && buf[i + 1] == '/') &&
							  (buf[i] != buf[i + 1]) && (i == 0 || buf[i - 1] != '\\') && !is_quote(buf[i + 1])))
		{
			if ((position = func_comp1(buf + i, &token_node, env_list, coord)) != -1)
				i += position;
			else
				return (NULL);
		}
		else if ((buf[i] && ft_is_there(PIPE, buf[i])) || (ft_is_there(AGG_REDI, buf[i]) && buf[i]))
		{
			if ((position = fun_comp2(buf + i, &token_node, coord)) != -1)
				i += position;
			else
				return (NULL);
		}
		else if (((buf[i] == '\'' || buf[i] == '\"') || (((buf[i] == '$' && is_quote(buf[i + 1]))) && (i == 0 || buf[i - 1] != '\\') && (i == 0 || buf[i - 1] == ';'))) ||
				 (!ft_is_there(METACHARACTER, buf[i]) && buf[i] && !is_quote(buf[i])))
		{
			if ((position = fun_comp3(buf + i, &token_node, coord, buf_len)) != -1)
				i += position;
			else
				return (NULL);
		}
		i++;
	}
	return (token_node);
}
*/





t_quote *quote_handling(char *s, char quote, int start)
{
	t_mystruct v;
	t_quote *quot;
	t_quote *rec_quote;

	ft_bzero(&v, sizeof(t_mystruct));
	if (!(quot = (t_quote *)ft_memalloc(sizeof(t_quote))))
		return (NULL);
	if (!(quot->string = ft_strnew(ft_strlen(s))))
		return (NULL);
	if (!*s)
		return (quote_completion(&quot, quote));
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
			if (s[i + 1] == '>' || s[i + 1] == '<' || s[i + 1] == '|')
				i = i + (ft_strchr(s + i, quote) - (s + i));
			else if ((!is_blank(s[i + 1]) || s[i + 1] == '\n') && (!ft_is_there(METACHARACTER, s[i + 1]) || s[i + 1] == '\n'))
			{
				if (!(rec_quote = quote_handling(s + i + 1, s[i], !start)))
					return (NULL);
				tmp = quot->string;
				quot->string = ft_strjoin(quot->string, rec_quote->string);
				i = i + rec_quote->size + 1;
				ft_strdel(&(rec_quote->string));
				ft_memdel((void **)&rec_quote);
				ft_strdel(&tmp);
				break;
			}
			else if (s[i] != quote || !start)
				return (quote_completion(&quot, s[i]));
			i++;
			break;
		}
		else
		{
			quot->string[j++] = s[i];
			if (s[i + 1] == '>' || s[i + 1] == '<')
			{
				i++;
				break;
			}
			flag = false;
		}
		if (!s[i + 1] && start)
			return (quote_completion(&quot, quote));
		i++;
	}
	quot->size = i;
	return (quot);
}
