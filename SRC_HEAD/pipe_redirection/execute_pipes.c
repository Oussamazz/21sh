/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:50:27 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/01 10:20:44 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		execute_pipes2(t_miniast *tree, t_mypipe *pipes)
{
	close(pipes->pipe[0]);
	if (pipes->cmd_no != 0)
	{
		dup2(pipes->temp, STDIN_FILENO);
		close(pipes->temp);
	}
	if (tree->pipe)
		dup2(pipes->pipe[1], STDOUT_FILENO);
	close(pipes->pipe[1]);
	return ;
}

static void		execute_pip_child(t_miniast *tree, t_mypipe *pipes,
	char **tabs, t_env **env_list)
{
	execute_pipes2(tree, pipes);
	if (tree->redirection)
		execute_redirection(tree->redirection, g_tty_name);
	if (!tree->pipe && pipes->cmd_no)
		close(pipes->temp);
	if (check_builtins(tree->cmd[0]))
		execute_blt_with_fork(tree->cmd, tabs, env_list);
	else if (tree->cmd[0][0] == '/' ||
		(tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/'))
		execute_direct(tree->cmd, tabs);
	else if (tree->cmd[0] && env_list)
		execute_undirect(tree->cmd, tabs, env_list);
	exit(EXIT_SUCCESS);
}

static void		execute_pipes1(t_miniast *tree, t_mypipe *pipes,
	char **tabs, t_env **env_list)
{
	if (tree->cmd && tree->cmd[0] && check_builtins_nfrk(tree->cmd[0]))
		return (execute_blt_with_fork(tree->cmd, tabs, env_list));
	if (pipe(pipes->pipe) == -1)
		return ;
	if ((pipes->pid = fork()) == -1)
		return ;
	if (pipes->pid == 0)
		execute_pip_child(tree, pipes, tabs, env_list);
	else
	{
		close(pipes->pipe[1]);
		if (pipes->temp)
			close(pipes->temp);
		pipes->temp = pipes->pipe[0];
		if (!tree->pipe)
			close(pipes->temp);
		pipes->cmd_no += 1;
	}
	return ;
}

static void		init_pipes(t_mypipe *pipes)
{
	pipes->temp = 0;
	pipes->cmd_no = 0;
	pipes->pipe[0] = 0;
	pipes->pipe[1] = 0;
	pipes->pid = 0;
}

int				execute_pipes(t_miniast *tree, char **tabs, t_env **env_list)
{
	t_mypipe	pipes;
	int			fd;

	init_pipes(&pipes);
	fd = 0;
	while (tree)
	{
		execute_pipes1(tree, &pipes, tabs, env_list);
		if (tree->sep)
		{
			while (wait(NULL) > 0)
				;
			g_ex_flag = 1;
			return (execute(tree->sep, env_list));
		}
		tree = tree->pipe;
	}
	close(pipes.temp);
	if (pipes.pid)
		while (wait(NULL) > 0)
			;
	return (255);
}
