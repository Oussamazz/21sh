/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/22 19:46:18 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int	ft_str_is_digit(char *lfd)
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
		return (1);
	}
	return (0);
}

int  ft_redirect_in_out_2(int fd, char *left_fd, t_redir *redirections)
{
	int left;

	left = 0;
	if (left_fd)
	{
		if (ft_str_is_digit(left_fd))
			left = ft_atoi(left_fd);
		else
			return (ft_putendl_fd_int("21sh: Error: Left redirection must only contains digits characters.", 2, -1));
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

int		ft_redirect_in_out(t_redir *redirections, t_redir *prev, int fd)
{
	char 	*right_fd;
	char 	*left_fd;

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
			ft_putendl_fd_error("21sh: No such file or directory: ", right_fd, "\n", NULL);
			exit(0);
			return (fd);
		}
	}
	ft_redirect_in_out_2(fd, left_fd, redirections);
	return (255);
}

int		append_redir(t_redir *redirection, t_redir *prev)
{
	int fd;
	int left;
	char *left_fd;
	char *right_fd;
	
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

int	here_document(t_redir *redirection, char *tty_name)
{
	int pip[2];
	int fd;
	int tmp;

	tmp = 255;
	if((fd = open(tty_name, O_RDWR)) == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	dup2(STDOUT_FILENO, tmp);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if ((pipe(pip)) == -1)
		return (-2);
	if (redirection->next && redirection->next->rfd)
		ft_putstr_fd(redirection->next->rfd, pip[1]);
	else if (redirection->next == NULL)
		return (ft_putendl_fd_int("21sh: parse error near `\\n'", 2, -3));
	close(pip[1]);
	dup2(pip[0], STDIN_FILENO);
	close(pip[0]);
	dup2(tmp, STDOUT_FILENO);
	close(tmp);
	return (255);
}

int		execute_redirection(t_redir *redirections, char *tty_name)
{
	t_redir *prev;
	int fd;

	fd = 0;
	prev = NULL;
	if (!tty_name)
		exit (1); // tty_name not found {errors must be handled};
	while (redirections != NULL)
	{
		if (redirections->sym && ft_is_there(redirections->sym, '&'))
			fd = agg_redirection(redirections, prev, fd);
		else if (redirections->sym && (ft_strequ(redirections->sym , ">") || ft_strequ(redirections->sym , "<"))) // REDIR_IN REDIR_OUT
			fd = ft_redirect_in_out(redirections, prev, fd);
		else if (redirections->sym && (ft_strequ(redirections->sym , ">>"))) // APPEND_OUT
			fd = append_redir(redirections, prev);
		else if (redirections->sym && (ft_strequ(redirections->sym , "<<"))) // HERE_DOC
			fd = here_document(redirections, g_tty_name);
		if (fd < 0)
			break ;
		prev = redirections;
		redirections = redirections->next;
	}
	return (fd);
}

void ft_reset_fd(char *tty_name, int file_d)
{
	int		fd;

	if ((fd = open(tty_name, O_RDWR)) == -1)
		return ;
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	if (file_d > 2)
		close(file_d);
	if (fd > 2)
		close(fd);
}

int				execute(t_miniast *tree, t_env **env_list)
{
	char	**tabs;
	int		fd;

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