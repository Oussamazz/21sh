/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 21:17:08 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void			ft_free_his(t_his **g_his)
{
	t_his		*cur;
	t_his		*tmp;

	if (g_his)
	{
		cur = *g_his;
		while (cur)
		{
			tmp = cur;
			ft_strdel(&(cur->data));
			cur = cur->next;
			free(tmp);
		}
		*g_his = NULL;
	}
}

static void		free_list_redir(t_redir *redirections)
{
	t_redir		*next;

	next = NULL;
	while (redirections)
	{
		if (redirections->lfd)
			ft_strdel(&redirections->lfd);
		if (redirections->sym)
			ft_strdel(&redirections->sym);
		if (redirections->rfd)
			ft_strdel(&redirections->rfd);
		next = redirections->next;
		if (redirections)
			free(redirections);
		redirections = next;
	}
	redirections = NULL;
	return ;
}

void			ft_free_tree(t_miniast **tree)
{
	if (!*tree)
		return ;
	ft_free_arr((*tree)->cmd);
	if ((*tree)->redirection)
		free_list_redir((*tree)->redirection);
	ft_free_tree(&(*tree)->pipe);
	ft_free_tree(&(*tree)->sep);
	ft_memdel((void **)tree);
}

void			ft_free_tokenz(t_lexer **head)
{
	t_lexer		*cur;
	t_lexer		*tmp;

	if (head && *head)
	{
		cur = *head;
		while (cur)
		{
			tmp = cur;
			if (cur->data)
				ft_strdel(&(cur->data));
			cur = cur->next;
			free(tmp);
		}
		*head = NULL;
	}
	return ;
}

void			free_vars(t_mystruct *v, int *to_free, int size)
{
	int			i;

	i = 0;
	while (i < size)
	{
		if (to_free[i] == F_TMP)
			ft_strdel(&v->tmp);
		else if (to_free[i] == F_TOKENZ)
			ft_free_tokenz(&v->tokenz);
		else if (to_free[i] == F_AST)
			ft_free_tree(&v->ast);
		else if (to_free[i] == F_G_HIS)
			ft_free_his(&g_his);
		else if (to_free[i] == F_STR)
			ft_strdel(&v->str);
		i++;
	}
}
