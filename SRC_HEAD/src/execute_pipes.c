/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:50:27 by macos             #+#    #+#             */
/*   Updated: 2020/12/21 17:23:46 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

 // ls -la | cat -e 

static void     execute_pipes2(t_miniast *tree, t_mypipe *pipes)
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

static int  check_builtins_nfrk(char *cmd_name)
{
    if (!ft_strcmp(cmd_name, "cd") || !ft_strcmp(cmd_name, "setenv") ||
     !ft_strcmp(cmd_name, "unsetenv") || !ft_strcmp(cmd_name, "exit"))
        return (1);
    return (0);
}

static void    execute_pipes1(t_miniast *tree, t_mypipe *pipes, char **tabs, t_env **env_list)
{
    if (tree->cmd && check_builtins_nfrk(tree->cmd[0]))
        return execute_blt_without_fork(tree, tree->cmd, tabs, env_list);
    if (pipe(pipes->pipe) == -1) // err
        return ;
    if ((pipes->pid = fork()) == -1)
        return ;                 // err
    if (pipes->pid == 0) // child proccess:
    {
        execute_pipes2(tree, pipes);
        if (tree->redirection)
            execute_redirection(tree->redirection, g_tty_name);
        if (!tree->pipe && pipes->cmd_no)
            close(pipes->temp);
        if (!ft_strcmp(tree->cmd[0], "echo") ||
         !ft_strcmp(tree->cmd[0], "env") || !ft_strcmp(tree->cmd[0], "type"))
            execute_blt_with_fork(tree, tree->cmd, tabs, env_list);
        else if (tree->cmd[0][0] == '/' || (tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/'))
            execute_direct(tree->cmd, tabs);
        else
            execute_undirect(tree->cmd, tabs, env_list);
        exit(EXIT_SUCCESS);
    }
    else // parent proccess:
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

static void init_pipes(t_mypipe *pipes)
{
    pipes->temp = 0;
    pipes->cmd_no = 0;
    pipes->pipe[0] = 0;
    pipes->pipe[1] = 0;
    pipes->pid = 0;
}

int				execute_pipes(t_miniast *tree, char **tabs, t_env **env_list)
{
    t_mypipe pipes;
    int fd;

    init_pipes(&pipes);
    fd = 0;
    while (tree)
    {
        execute_pipes1(tree, &pipes, tabs, env_list);
        if (tree->sep)
        {
            while(wait(NULL) > 0)
                ;
            g_ex_flag = 1;
            return (execute(tree->sep, env_list));
        }
        tree = tree->pipe;
    }
    close(pipes.temp);
    if (pipes.pid)
        while(wait(NULL) > 0)
            ;
    return (255);
}

/*

int				execute(t_miniast *tree, t_env **env_list)
{
	t_miniast *sepa;
	char	**tabs;
	int		status;
	int		fd;

	status = 1;
	fd = 0;
	if (!(tabs = list_to_tabs(env_list)))
        return (0);
	while (tree != NULL)
	{
		if (tree->pipe)
			fd = execute_pipes(tree, tabs, env_list); // parent process (no fork): cd setenv unsetenv exit 
        else
        {
			if (tree->redirection)
				fd = execute_redirection(tree->redirection, g_tty_name);
            if (fd >= 0 && (tree->cmd[0][0] == '/' || (tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/')))
                execute_direct(tree->cmd, tabs);
            else if (fd >= 0)
                execute_undirect(tree->cmd, tabs, env_list);
        }
		ft_reset_fd(g_tty_name, fd);
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (status);
}

*/