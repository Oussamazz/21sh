/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   agg_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:53:48 by macos             #+#    #+#             */
/*   Updated: 2020/12/16 21:05:35 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int ft_agg_digit(t_redir *redirection, int fd, int lfd)
{
    if (redirection->rfd && ft_str_is_digit(redirection->rfd))
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

    if (!redirection->rfd)
        return (-1);
	if (!(file_d = ft_strsub(redirection->rfd, 0, ft_strlen(redirection->rfd) - 1)))
        return (-1);
	fd = ft_atoi(file_d);
	if (fd != lfd)
	{
		ft_strdel(&file_d);
        if (dup2(fd, lfd) < 0)
        {
            ft_putstr_fd("21sh: ", 2);
			ft_putnbr_fd(fd, 2);
			ft_putendl_fd(": Bad file descriptor\n", 2);
			return (-1);
        }
		close(fd);
	}
    else
        ft_strdel(&file_d);
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
		if (prev->lfd == NULL)
			dup2(fd, 2);
	}
	close(fd);
	return (fd);
}

int		ft_agg_out(t_redir *redir, t_redir *prev, int fd)
{
	int		left;

	if (prev && prev->lfd && ft_str_is_digit(prev->lfd))
		left = ft_atoi(prev->lfd);
	else
		left = 1;
	if (redir->next && redir->next->rfd != NULL && ft_strcmp(redir->sym, ">&-"))
		fd = ft_agg_digit(redir->next, fd, left);
	else if (ft_strcmp(redir->sym, ">&-") == 0)
	{
		close(left);
		return (255);
	}
	else if (redir->next && redir->next->rfd && ft_strlen(redir->next->rfd) > 1 && redir->next->rfd[ft_strlen(redir->next->rfd) - 1] == '-')
		fd = ft_agg_close(redir->next, fd, left);
	else if (redir->next)
		fd = ft_agg_word(redir->next, prev, fd, left);
	return (fd);
}

int		ft_agg_in(t_redir *redir, t_redir *prev, int fd)
{
	int left;

	if (prev && prev->lfd != NULL && ft_str_is_digit(prev->lfd))
		left = ft_atoi(prev->lfd);
	else
		left = 0;
	if (redir->next && redir->next->rfd && ft_strcmp(redir->sym, "<&-"))
		fd = ft_agg_digit(redir->next, fd, left);
	else if (ft_strcmp(redir->sym, "<&-") == 0)
	{
		close(left);
		return (255);
	}
	else if (redir->next && ft_strlen(redir->next->rfd) > 1 && redir->next->rfd[ft_strlen(redir->next->rfd) - 1] == '-')
		fd = ft_agg_close(redir->next, fd, left);
	else if (redir->next)
		fd = ft_agg_word(redir->next, prev, fd, left);
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