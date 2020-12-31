/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agg_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:53:48 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			ft_agg_digit(t_redir *redirection, int fd, int lfd)
{
	if (redirection->rfd)
		fd = ft_atoi(redirection->rfd);
	if (fd != lfd)
	{
		if (dup2(fd, lfd) < 0)
		{
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(fd, 2);
			ft_putendl_fd(": Bad file descriptor\n", 2);
			return (-1);
		}
	}
	return (fd);
}

int			ft_agg_close(t_redir *redirection, int fd, int lfd)
{
	char	*file_d;

	file_d = NULL;
	if (!redirection->rfd)
		return (-1);
	if (!(file_d = ft_strsub(redirection->rfd, 0,
		ft_strlen(redirection->rfd) - 1)))
		return (-1);
	fd = ft_atoi(file_d);
	ft_strdel(&file_d);
	if (fd != lfd)
	{
		if (dup2(fd, lfd) < 0)
		{
			ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(fd, 2);
			ft_putendl_fd(": Bad file descriptor\n", 2);
			return (-1);
		}
		close(fd);
	}
	return (fd);
}

int			ft_agg_word(t_redir *redirection, t_redir *prev, int fd, int lfd)
{
	if (!redirection->rfd)
		return (-1);
	if ((fd = open(redirection->rfd, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
		return (-1);
	if (fd != lfd)
	{
		dup2(fd, lfd);
		if (prev == NULL)
			dup2(fd, 2);
	}
	close(fd);
	return (fd);
}

int			agg_redirection(t_redir *redirections, t_redir *prev, int fd)
{
	if (redirections->sym && redirections->sym[0] == '>')
		fd = ft_agg_out(redirections, prev, fd);
	else if (redirections->sym && redirections->sym[0] == '<')
		fd = ft_agg_in(redirections, prev, fd);
	return (fd);
}
