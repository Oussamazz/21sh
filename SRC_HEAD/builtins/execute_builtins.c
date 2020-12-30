/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:32:52 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/30 18:53:33 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int		check_varname(char *cmd)
{
	int		i;

	i = 0;
	if (!cmd || !ft_isalpha(*cmd))
		return (0);
	while (cmd[i])
	{
		if (i && !ft_isalnum(cmd[i]))
			return (0);
		i++;
	}
	return (1);
}

int				check_args_no(char **cmd)
{
	int			i;

	i = 0;
	while (cmd[i++] != NULL)
		;
	return (i - 1);
}

void			blt_unsetenv(char **cmd, t_env **env_list)
{
	int			i;

	if (check_args_no(cmd) < 2)
		return (ft_putendl_fd("21sh: Error: [unsetenv [var_name] ...].", 2));
	i = 1;
	while (cmd[i])
	{
		deleteNode(env_list, cmd[i]);
		i++;
	}
	return ;
}

void			blt_setenv(char **cmd, t_env **env_list)
{
	if ((check_args_no(cmd)) == 1)
		return (print_env_list(env_list));
	if ((check_args_no(cmd)) != 3)
		return (ft_putendl_fd("21sh: Error: [setenv [var_name] [var_value]].", 2));
	else if (!check_varname(cmd[1]))
		return (ft_putendl_fd("21sh: setenv: Variable name must begin with a letter and contains only alpha-numeric characters.", 2));
	if (env_exist(env_list, cmd[1]) == 0)
		addtolist(env_list, ft_strdup(cmd[1]), ft_strdup(cmd[2]));
	else
		modify_env(env_list, cmd[1], cmd[2]);
	return ;
}

void			execute_blt_with_fork(char **cmd, char **tabs, t_env **env_list)
{
	if (cmd && tabs && *env_list)
	{
		if (ft_strequ(cmd[0], "echo"))
			blt_echo(cmd);
		else if (ft_strequ(cmd[0], "env"))
			print_env_list(env_list);
		else if (ft_strequ(cmd[0], "type"))
			type_builtin(cmd, env_list);
		else if (ft_strequ(cmd[0], "cd"))
			blt_cd(cmd, env_list);
		else if (ft_strequ(cmd[0], "setenv"))
			blt_setenv(cmd, env_list);
		else if (ft_strequ(cmd[0], "unsetenv"))
			blt_unsetenv(cmd, env_list);
		else if (ft_strequ(cmd[0], "exit"))
			exit(0);
	}
	return ;
}
