/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agg_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:53:48 by macos             #+#    #+#             */
/*   Updated: 2020/12/17 02:02:34 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int ft_agg_digit(t_redir *redirection, int fd, int lfd)
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

static int ft_agg_close(t_redir *redirection, int fd, int lfd)
{
    char *file_d;

    file_d = NULL;
    if (!redirection->rfd)
        return (-1);
	if (!(file_d = ft_strsub(redirection->rfd, 0, ft_strlen(redirection->rfd) - 1)))
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

static int ft_agg_word(t_redir *redirection, t_redir *prev, int fd, int lfd)
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

int		ft_agg_out(t_redir *redir, t_redir *prev, int fd)
{
	int		left;
    t_redir *redir_n;

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
	else if (redir_n && redir_n->rfd && ft_strlen(redir_n->rfd) > 2 && redir_n->rfd[ft_strlen(redir_n->rfd) - 1] == '-')
		fd = ft_agg_close(redir_n, fd, left);
	else if (redir_n)
		fd = ft_agg_word(redir_n, prev, fd, left);
	return (fd);
}

int		ft_agg_in(t_redir *redir, t_redir *prev, int fd)
{
	int left;
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
	else if (redir_n && ft_strlen(redir_n->rfd) > 1 && redir_n->rfd[ft_strlen(redir_n->rfd) - 1] == '-')
		fd = ft_agg_close(redir_n, fd, left);
	else if (redir_n)
		fd = ft_agg_word(redir_n, prev, fd, left);
	return (fd);
}

int agg_redirection(t_redir *redirections, t_redir *prev, int fd)
{
    if (redirections->sym && redirections->sym[0] == '>')
        fd = ft_agg_out(redirections, prev, fd);
    else if (redirections->sym && redirections->sym[0] == '<')
        fd = ft_agg_in(redirections, prev, fd);
    return (fd);
}