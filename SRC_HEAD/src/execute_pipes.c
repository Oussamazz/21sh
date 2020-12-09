/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:50:27 by macos             #+#    #+#             */
/*   Updated: 2020/12/09 14:58:49 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static void    execute_pipes1(t_miniast *tree, t_mypipe *pipes, char **tabs, t_env **env_list)
{
    
}

static void init_pipes(t_mypipe *pipes)
{
    pipes->temp = 0;
    pipes->cmd_no = 0;
    pipes->pipe[0] = -1;
    pipes->pipe[1] = -1;
    pipes->pid = -1;
}

int				execute_pipes(t_miniast *tree, char **tabs, t_env **env_list)
{
    t_mypipe pipes;

    init_pipes(&pipes);
    while (tree)
    {
        execute_pipes1(tree, &pipes, tabs, env_list);
        tree = tree->pipe;
    }
    close(pipes.temp);
    if (pipes.pid)
    {
        while (wait(NULL))
        {
        }
    }
    return (255);
}