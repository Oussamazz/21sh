/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:50:27 by macos             #+#    #+#             */
/*   Updated: 2020/12/12 00:20:24 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

 // ls -la | cat -e 

static void     execute_pipes2(t_miniast *tree, t_mypipe *pipes)
{
    close(pipes->pipe[0]);
    if (pipes->cmd_no != 0)
        dup2(pipes->temp, STDIN_FILENO);
    close(pipes->temp);
    if (tree->pipe)
        dup2(pipes->pipe[1], STDOUT_FILENO);
    close(pipes->pipe[1]);
    if (tree->redirection)
        execute_redirection(tree->redirection, g_tty_name);
    return ;
}

static void    execute_pipes1(t_miniast *tree, t_mypipe *pipes, char **tabs, t_env **env_list)
{
    if (pipe(pipes->pipe) == -1) // err
        return ;
    if ((pipes->pid = fork()) == -1)
        return ;                 // err
    if (pipes->pid == 0) // child proccess:
    {
        execute_pipes2(tree, pipes);
        // if (tree->cmd[0][0] == '/' || (tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/')) // ./test.sh prob
        //     execute_direct(tree->cmd, tabs);
        //else
        execute_undirect(tree->cmd, tabs, env_list);
        //execute(tree, env_list, 0);
        exit(EXIT_SUCCESS);
    }
    else // parent proccess:
    {
        close(pipes->pipe[1]);
        if (pipes->cmd_no != 0)
            close(pipes->temp);
        pipes->temp = pipes->pipe[0];
        pipes->cmd_no += 1;
    }
    wait(NULL);
    return ;
}

static void init_pipes(t_mypipe *pipes)
{
    pipes->temp = 0;
    pipes->cmd_no = 0;
    pipes->pipe[0] = -1;
    pipes->pipe[1] = -1;
    pipes->pid = -1;
}

int				execute_pipes(t_miniast *tree, char **tabs, t_env **env_list) // ls -la | wc -l ; echo oussa
{
    t_mypipe pipes;

    init_pipes(&pipes);
    while (tree)
    {
        if (tree->sep)
        {
            execute_pipes1(tree, &pipes, tabs, env_list);
            execute(tree->sep, env_list, 1);
            break ;
        }
        execute_pipes1(tree, &pipes, tabs, env_list);
        tree = tree->pipe;
    }
    close(pipes.temp);
    return (255);
}