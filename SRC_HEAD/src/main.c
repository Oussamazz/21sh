/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 17:13:38 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/27 01:26:04 by oelazzou            ###   ########.fr       */
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
		cor->aggr_index = 1;
		cor->pipe_index = 1;
	}
	return;
}

static void	flag_g(char **av, t_env **env_list, time_t *now)
{
	char *user;

	user = NULL;
	if (!(user = get_value_expansion("USER", env_list)))
		error_message("21sh: Error: USER NOT FOUND.\n", 1);
	starting_message(av, &user, now);
	return ;
}

int main(int ac, char **av, char **env)
{
	time_t now;
	t_env *env_list;

	env_list = NULL;
	stock_env(env, &env_list);
	time(&now);
	if (ac > 1 && env_list)
		flag_g(av, &env_list, &now);
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
}

void print_his(t_his *g_his)
{
	while (g_his)
	{
		ft_putstr_fd(g_his->data, 1);
		g_his = g_his->next;
	}
}

char *handel_signal(char c)
{
	if (g_clt_d)
	{
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putchar_fd(c, 2);
		ft_putendl_fd("\"", 2);
	}
	return (NULL);
}

static void	assign_v(t_getfullcmd *v)
{
	v->flag = 1;
	v->quote_opened ^= 1;
	v->c = v.tmp[v->i] * v->quote_opened;
}

char *get_full_cmd()
{
	t_getfullcmd v;

	ft_bzero(&v, sizeof(t_getfullcmd));
	v.cmd = ft_strdup("");
	while(v.cmd && (v.tmp = ft_readline(v.flag)))
	{
		v.i = 0;
		while(v.tmp[v.i])
		{
			if(is_quote(v.tmp[v.i]) && (v.c == v.tmp[v.i] || v.c == 0))
			{
				v.flag = 1;
				v.quote_opened ^= 1;
				v.c = v.tmp[v.i] * v.quote_opened;
			}
			v.i++;
		}
		v.cmd = ft_freejoin(v.cmd, v.tmp, 2);
		if (!v.quote_opened)
			break ;
		if (g_clt_d || g_clt_c)
			return(handel_signal(v.c));
		prompt_completion(v.c);
		v.cmd = ft_freejoin(v.cmd, "\n", 0);
	}
	return(v.tmp ? v.cmd : NULL);
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
		if (!(v.str = get_full_cmd()))
			continue ;
		add_to_his(v.str, &g_his, 0);
		v.tokenz = lexer(v.str, head, &v.coord);
		if (v.tokenz)
			print_list(v.tokenz);
		ft_putendl_fd("_____________", 1);
		v.status[1] = check_grammar_tokenz(v.tokenz);
		if (v.tokenz && head && v.status[1] > 0)
			v.status[1] = parse_commands(&v.ast, v.tokenz, head);
		if (v.str[0] != '\0' && !str_is_blank(v.str))
			add_to_history(v.tmp = join_all_bufs(g_his));
		if (v.status[1] > 0 && v.ast && head && v.ast->cmd)
			v.status[0] = execute(v.ast, head);
		free_vars(&v,(int[]){F_TMP, F_TOKENZ, F_AST, F_STR, F_G_HIS}, 5);
	}
}
