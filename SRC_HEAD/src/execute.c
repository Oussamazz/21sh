/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 03:16:16 by macos             #+#    #+#             */
/*   Updated: 2020/12/06 15:54:27 by macos            ###   ########.fr       */
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
	if (!(tabs = list_to_tabs(env_list)))
        return (0);
	while (tree)
	{
		if (tree->redirection)
        {
            
        }
		tree = tree->sep;
	}
	ft_free_arr(tabs);
	return (status);
}