/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agg_in_out.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:28:11 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			ft_agg_out(t_redir *redir, t_redir *prev, int fd)
{
	int		left;
	t_redir	*redir_n;

	redir_n = redir->next;
	if (prev && prev->lfd && ft_str_is_digit(prev->lfd))
		left = ft_atoi(prev->lfd);
	else
		left = 1;
	if (redir_n && redir_n->rfd &&
		ft_strcmp(redir->sym, ">&-") != 0 &&
		ft_str_is_digit(redir_n->rfd))
		fd = ft_agg_digit(redir_n, fd, left);
	else if (ft_strcmp(redir->sym, ">&-") == 0)
	{
		close(left);
		return (255);
	}
	else if (redir_n && redir_n->rfd && ft_strlen(redir_n->rfd) > 2 &&
		redir_n->rfd[ft_strlen(redir_n->rfd) - 1] == '-')
		fd = ft_agg_close(redir_n, fd, left);
	else if (redir_n)
		fd = ft_agg_word(redir_n, prev, fd, left);
	return (fd);
}

int			ft_agg_in(t_redir *redir, t_redir *prev, int fd)
{
	int		left;
	t_redir *redir_n;

	redir_n = redir->next;
	if (prev && prev->lfd != NULL && ft_str_is_digit(prev->lfd))
		left = ft_atoi(prev->lfd);
	else
		left = 0;
	if ((redir_n && redir_n->rfd) &&
		ft_strcmp(redir->sym, "<&-") != 0 &&
		ft_str_is_digit(redir_n->rfd))
		fd = ft_agg_digit(redir_n, fd, left);
	else if (ft_strcmp(redir->sym, "<&-") == 0)
	{
		close(left);
		return (255);
	}
	else if (redir_n && ft_strlen(redir_n->rfd) > 1 &&
		redir_n->rfd[ft_strlen(redir_n->rfd) - 1] == '-')
		fd = ft_agg_close(redir_n, fd, left);
	else if (redir_n)
		fd = ft_agg_word(redir_n, prev, fd, left);
	return (fd);
}
