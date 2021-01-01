/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 10:13:51 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int				ft_redirect_in_out_2(int fd, char *left_fd,
	t_redir *redirections)
{
	int			left;

	left = 0;
	if (left_fd)
	{
		if (ft_str_is_digit(left_fd))
			left = ft_atoi(left_fd);
		else
		{
			ft_putstr_fd(ERROR4, 2);
			ft_putendl_fd(ERROR44, 2);
			return (-1);
		}
		dup2(fd, left);
	}
	else
	{
		if (!ft_strcmp(redirections->sym, ">"))
			dup2(fd, STDOUT_FILENO);
		else if (!ft_strcmp(redirections->sym, "<"))
			dup2(fd, STDIN_FILENO);
	}
	close(fd);
	return (0);
}

int				ft_redirect_in_out(t_redir *redirections, t_redir *prev, int fd)
{
	char		*right_fd;
	char		*left_fd;

	left_fd = NULL;
	if (redirections->next)
		right_fd = redirections->next->rfd;
	if (prev)
		left_fd = prev->lfd;
	if (ft_strcmp(redirections->sym, ">") == 0 && right_fd)
		fd = open(right_fd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (right_fd)
	{
		fd = open(right_fd, O_RDONLY);
		if (fd < 0)
		{
			ft_putendl_fd_error("21sh: No such file or directory: ", right_fd,
				"\n", NULL);
			exit(0);
			return (fd);
		}
	}
	ft_redirect_in_out_2(fd, left_fd, redirections);
	return (255);
}

int				append_redir(t_redir *redirection, t_redir *prev)
{
	int			fd;
	int			left;
	char		*left_fd;
	char		*right_fd;

	left_fd = NULL;
	right_fd = NULL;
	if (redirection)
		right_fd = redirection->next->rfd;
	if (prev)
		left_fd = prev->lfd;
	if ((fd = open(right_fd, O_CREAT | O_WRONLY | O_APPEND, 0644)) == -1)
		return (-1);
	if (left_fd)
	{
		if (ft_str_is_digit(left_fd))
			left = ft_atoi(left_fd);
		else
			return (ft_putendl_fd_int("21sh: Error: Left redirection.", 2, -1));
		dup2(fd, left);
	}
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (fd);
}

int				execute_redirection(t_redir *redirections, char *tty_name)
{
	t_redir		*prev;
	int			fd;

	fd = 0;
	prev = NULL;
	if (!tty_name)
		exit(1);
	while (redirections != NULL)
	{
		if (redirections->sym && ft_is_there(redirections->sym, '&'))
			fd = agg_redirection(redirections, prev, fd);
		else if (redirections->sym && (ft_strequ(redirections->sym, ">") ||
			ft_strequ(redirections->sym, "<")))
			fd = ft_redirect_in_out(redirections, prev, fd);
		else if (redirections->sym && (ft_strequ(redirections->sym, ">>")))
			fd = append_redir(redirections, prev);
		else if (redirections->sym && (ft_strequ(redirections->sym, "<<")))
			fd = here_document(redirections, g_tty_name);
		if (fd < 0)
			break ;
		prev = redirections;
		redirections = redirections->next;
	}
	return (fd);
}

int				execute(t_miniast *tree, t_env **env_list)
{
	char		**tabs;
	int			fd;

	fd = 0;
	if (!(tabs = list_to_tabs(env_list)))
		return (0);
	while (tree != NULL && tree->cmd[0])
	{
		fd = execute_pipes(tree, tabs, env_list);
		ft_reset_fd(g_tty_name, fd);
		if (g_ex_flag)
			break ;
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (1);
}
