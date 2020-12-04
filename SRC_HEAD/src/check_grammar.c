/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_grammar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 01:34:34 by macos             #+#    #+#             */
/*   Updated: 2020/12/04 02:31:03 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int error_syntax_redirections(void)
{
    ft_putendl_fd("21sh: syntax error near unexpected token.", 2);
    return (0);
}

static	int	check_redirection_syntax(t_redir *redirection)
{
    size_t pos;
    char *str;
	t_redir	*red;

	red = redirection;
	while (red)
	{
        str = NULL;
        pos = 0;
        if (red->lfd)
        {
            if (red->next->sym == NULL)
                return (error_syntax_redirections());
        }
        else if (red->sym)
        {
            str = red->sym;
            pos = ft_strlen(str) - 1;
            if (red->next == NULL && str[pos] != '-')
                return (error_syntax_redirections());
        }
        else if (red->rfd)
        {
            if (red->next != NULL)
                return (error_syntax_redirections());
        }
		red = red->next;
	}
	return (1);
}

static	int	check_pipe_syntax(t_miniast *ast)
{
	t_miniast *current;

	current = ast;
	while (current)
	{
		if (current->redirection)
		{
			if (!check_redirection_syntax(current->redirection))
				return (0);
		}
		if (!current->cmd)
			return (error_syntax_redirections());
		current = current->pipe;
	}
	return (1);
}

int			check_grammar_tree(t_miniast *tree)
{
	if (!tree)
		return (0);
	while (tree)
	{
		if (tree->pipe != NULL)
		{
			if (!check_pipe_syntax(tree))
				return (0);
		}
		else if (tree->cmd == NULL)
            return (error_syntax_redirections());
		if (tree->redirection != NULL)
		{
			if (!check_redirection_syntax(tree->redirection))
				return (0);
		}
		tree = tree->sep;
	}
	return (1);
}
