/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 17:13:38 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 14:17:24 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		init_coord(t_pointt *cor)
{
	if (cor)
	{
		cor->node_index = 0;
		cor->node_addr = 0;
		cor->no_space = 0;
		cor->aggr_index = 1;
		cor->pipe_index = 1;
	}
	return ;
}

int				main(int ac, char **av, char **env)
{
	time_t		now;
	t_env		*env_list;

	env_list = NULL;
	stock_env(env, &env_list);
	time(&now);
	if (ac > 1 && env_list)
		flag_g(av, &env_list, &now);
	if (!(g_tty_name = ttyname(0)))
		return (1);
	source_sh(&env_list);
	env_list = NULL;
	if (g_tty_name)
		free(g_tty_name);
	return (0);
}

static void		assign_v(t_getfullcmd *v)
{
	v->flag = 1;
	v->quote_opened ^= 1;
	v->c = v->tmp[v->i] * v->quote_opened;
}

char			*get_full_cmd(void)
{
	t_getfullcmd v;

	ft_bzero(&v, sizeof(t_getfullcmd));
	v.cmd = ft_strdup("");
	while (v.cmd && (v.tmp = ft_readline(v.flag)))
	{
		v.i = 0;
		while (v.tmp[v.i])
		{
			if (is_quote(v.tmp[v.i]) && (v.c == v.tmp[v.i] || v.c == 0))
				assign_v(&v);
			v.i++;
		}
		v.cmd = ft_freejoin(v.cmd, v.tmp, 2);
		if (!v.quote_opened)
			break ;
		if (g_clt_d || g_clt_c)
			return (handel_signal(&v));
		prompt_completion(v.c);
		v.cmd = ft_freejoin(v.cmd, "\n", 0);
	}
	return (v.tmp ? v.cmd : NULL);
}

void			source_sh(t_env **head)
{
	t_mystruct	v;

	signal(SIGINT, ft_ctrlc);
	g_his = NULL;
	ft_bzero(&v, sizeof(t_mystruct));
	v.status[0] = 1;
	while (v.status[0] && g_tty_name)
	{
		init_coord(&v.coord);
		ft_prompte();
		if (!(v.str = get_full_cmd()))
			continue;
		v.tokenz = lexer(v.str, head, &v.coord);
		v.status[1] = check_grammar_tokenz(v.tokenz);
		if (v.tokenz && head && v.status[1] > 0)
			v.status[1] = parse_commands(&v.ast, v.tokenz, head);
		if (v.str[0] != '\0' && !str_is_blank(v.str))
			add_to_history(v.str);
		if (v.status[1] > 0 && v.ast && head && v.ast->cmd)
			v.status[0] = execute(v.ast, head);
		free_vars(&v, (int[]){F_TMP, F_TOKENZ, F_AST, F_STR, F_G_HIS}, 5);
	}
}
