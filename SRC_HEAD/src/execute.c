/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by macos             #+#    #+#             */
/*   Updated: 2020/12/07 18:44:55 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int	ft_str_is_digit(char *lfd)
{
	int i;

	if (lfd)
	{
		i = 0;
		while (lfd[i])
		{
			if (!ft_isdigit(lfd[i]))
				return (0);
			i++;
		}
		ft_putendl_fd("some", 1);
		return (1);
	}
	return (0);
}

int		ft_redirect_in_out(t_redir *redirections, t_redir *prev, int fd)
{
	int		left;
	char 	*right_fd;
	char 	*left_fd;

	left = 0;
	left_fd = NULL;
	if (redirections->next)
		right_fd = redirections->next->rfd;
	if (prev)
		left_fd = prev->lfd;
	if (!ft_strcmp(redirections->sym, ">") && right_fd)
	{
		fd = open(right_fd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else if (right_fd)
	{
		fd = open(right_fd, O_RDONLY);
		if (fd < 0)
			ft_putendl_fd("21sh: Error: Redirections", 2);
	}
	if (left_fd)
	{
		if (ft_str_is_digit(left_fd))
			left = ft_atoi(left_fd);
		else
		{
			ft_putendl_fd("21sh: Error: Left redirection.", 2);
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
	return (255);
}

int		execute_redirection(t_redir *redirections, char *tty_name)
{
	t_redir *prev;
	int fd;

	fd = 0;
	prev = NULL;
	if (!tty_name)
		exit (1); // tty_name error;
	while (redirections)
	{
		if (redirections->sym)
			fd = ft_redirect_in_out(redirections, prev, fd);
		prev = redirections;
		redirections = redirections->next;
	}
	return (fd);	
}

static void ft_reset_fd(char *tty_name, int file_d)
{
	int		fd;

	if ((fd = open(tty_name, O_RDWR)) == -1)
		return ;
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	if (file_d > 2)
		close(file_d);
	if (fd > 2)
		close(fd);
}

int				execute(t_miniast *tree, t_env **env_list)
{
	char	**tabs;
	int		status;
	int		fd;

	status = 1;
	fd = 0;
	if (!(tabs = list_to_tabs(env_list)))
        return (0);
	while (tree)
	{
		if (tree->redirection)
			fd = execute_redirection(tree->redirection, g_tty_name);
        if (tree->cmd && fd >= 0)
        {
            if (tree->cmd[0][0] == '/' || (tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/'))
                execute_direct(tree->cmd, tabs);
            else
                execute_undirect(tree->cmd, tabs, env_list);
			ft_reset_fd(g_tty_name, fd);
        }
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (status);
}