/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 19:46:15 by oelazzou             #+#    #+#             */
/*   Updated: 2020/11/27 20:35:07 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static	int	check_redirection_syntax(t_redir *redirection)
{
	t_redir	*redir;

	redir = redirection;
	while (redir)
	{
		if (!redir->lfd || !redir->sym || !redir->rfd)
		{
			ft_putendl_fd("21sh: syntax error near unexpected token", 2);
			return (1);
		}
		redir = redir->next;
	}
	return (0);
}

static	int	check_pipe_syntax(t_miniast *ast)
{
	while (ast)
	{
		if (ast->redirection)
		{
			if (check_redirection_syntax(ast->redirection))
				return (1);
		}
		if (!ast->cmd)
		{
			ft_putendl_fd("21sh: syntax error near unexpected token", 2);
			return (1);
		}
		ast = ast->pipe;
	}
	return (0);
}

int			check_syntax(t_miniast *ast)
{
	if (!ast)
		return (0);
	while (ast)
	{
		if (ast->pipe)
		{
			if (check_pipe_syntax(ast))
				return (0);
		}
		else if (!ast->cmd)
		{
			ft_putendl_fd("21sh: syntax error near unexpected token", 2);
			return (0);
		}
		if (ast->redirection)
		{
			if (check_redirection_syntax(ast->redirection))
				return (0);
		}
		ast = ast->sep;
	}
	return (1);
}