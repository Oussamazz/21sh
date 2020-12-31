/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_grammar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 19:15:08 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int				print_error_sym(t_type type)
{
	err_ret("21sh: syntax error near unexpected token `", NULL);
	if (type == PIPE_SYM)
		ft_putendl_fd("|'", 2);
	else if (type == SEP)
		ft_putendl_fd(";'", 2);
	else if (type == AGGR_SYM)
		ft_putendl_fd(">' or| `<'", 2);
	return (-1);
}

static int		check_token_type(t_type type, t_lexer *tokenz,
	t_lexer *next, int tokenz_size)
{
	if ((tokenz->type == type && next->coor.node_index <= tokenz_size
			&& (next->type == type)) ||
			(next->type != WORD && next->type != SQUOT &&
			next->type != DQUOT && next->type != EXPANSION))
	{
		print_error_sym(type);
		return (0);
	}
	return (1);
}

static int		check_grammar_tokenz_2(t_lexer *tokenz, int tokenz_size)
{
	if (tokenz->type == PIPE_SYM && tokenz->next)
		if (!(check_token_type(PIPE_SYM, tokenz,
			tokenz->next, tokenz_size)))
			return (0);
	if (tokenz->type == SEP && tokenz->next)
		if (!(check_token_type(SEP, tokenz, tokenz->next, tokenz_size)))
			return (0);
	return (1);
}

int				check_grammar_tokenz(t_lexer *tokenz)
{
	size_t		tokenz_size;

	if (tokenz)
	{
		tokenz_size = get_list_size(tokenz);
		while (tokenz && tokenz->coor.node_index <= (int)tokenz_size)
		{
			if (tokenz->type == AGGR_SYM && tokenz->coor.node_index == 1)
				return (print_error_sym(AGGR_SYM));
			if (tokenz->type == AGGR_SYM && !tokenz->next &&
				!(ft_strlen(tokenz->data) > 2))
				return (print_error_sym(AGGR_SYM));
			else if (tokenz->type == PIPE_SYM && (!tokenz->next ||
				tokenz->coor.node_index == 1))
				return (print_error_sym(PIPE_SYM));
			if (!(check_grammar_tokenz_2(tokenz, (int)tokenz_size)))
				return (0);
			tokenz = tokenz->next;
		}
		return (1);
	}
	return (0);
}
