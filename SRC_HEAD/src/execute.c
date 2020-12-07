/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by macos             #+#    #+#             */
/*   Updated: 2020/12/07 02:17:13 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"


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
		// if (tree->redirection)
        // {
            
        // }
        if (tree->cmd)
        {
            if (tree->cmd[0][0] == '/' || (tree->cmd[0][0] == '.' && tree->cmd[0][1] == '/'))
                execute_direct(tree->cmd, tabs);
            else
                execute_undirect(tree->cmd, tabs, env_list);
        }
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (status);
}