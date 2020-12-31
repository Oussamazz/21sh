/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:19:17 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void			fill_redirections(t_redir **node, t_lexer *token)
{
	t_redir *cur;
	t_redir *new;

	if (node)
	{
		if (!(new = (t_redir*)ft_memalloc(sizeof(t_redir))))
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

void			fill_cmd(char **ret, t_lexer *token, int *i, t_env **env)
{
	char		*tmp;

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
			(token->next->type == DQUOT || token->next->type == SQUOT ||
				token->type == EXPANSION))
		{
			tmp = token->next->data;
			token->next->data = ft_strjoin(ret[*i], tmp);
			free(ret[*i]);
			*i -= 1;
			free(tmp);
		}
	}
}

int				fill_cmd_redir(t_lexer *token, int *i, t_redir **redirections)
{
	if (token->type == AGGR_SYM ||
		token->type == L_REDIR || token->type == R_REDIR)
	{
		fill_redirections(redirections, token);
		*i -= 1;
	}
	else if (token->type == SEP || token->type == PIPE_SYM)
		return (1);
	return (0);
}
