/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by macos             #+#    #+#             */
/*   Updated: 2020/12/04 03:21:46 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int     execute_pipe()
{
       
}

int				execute(t_miniast *tree, t_env **env_list)
{
	char	**tabs;
	int		status;
	int		fd;

	status = 1;
	fd = 0;
	tabs = list_to_tabs(env_list);
	while (tree)
	{
		if (tree->pipe)
			fd = execute_pipe(tree, env_list, tabs);
		else
		{
			if (tree->redirection)
				fd = execute_redirection(tree->redirection);
			if (fd >= 0)
				status = execute_simple_cmd(tree->cmd, tabs, env_list);
			//ft_reset_fd(fd);
		}
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (status);
}