/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_blt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 18:52:44 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		print_blt(char *cmd_name)
{
	ft_putstr_fd(cmd_name, 1);
	ft_putstr_fd(" is a shell builtin.\n", 1);
	return ;
}

void			type_builtin(char **cmd, t_env **env_list)
{
	int		i;
	char	*bin_path;

	bin_path = NULL;
	i = 1;
	while (cmd[i] && env_list)
	{
		if (check_builtins(cmd[i]))
			print_blt(cmd[i]);
		else if ((bin_path = get_bin_file(cmd + i, env_list)))
		{
			ft_putstr_fd(cmd[i], 1);
			ft_putstr_fd(" is ", 1);
			ft_putendl_fd(bin_path, 1);
			ft_strdel(&bin_path);
		}
		else
			ft_putendl_fd_error("21sh: type: ", cmd[i], ": not found.\n", NULL);
		i++;
	}
	return ;
}
