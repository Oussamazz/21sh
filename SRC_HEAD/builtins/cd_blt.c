/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_blt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:59:22 by oelazzou          #+#    #+#             */
/*   Updated: 2021/01/18 15:05:57 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		cd_simple(char *path, t_env **env_list)
{
	struct stat	st;
	char		*tmp;
	char		*tmp2;

	if (path && access(path, F_OK) == 0)
	{
		stat(path, &st);
		if (!S_ISDIR(st.st_mode) && ft_strrchr(path, '/'))
		{
			return (ft_putendl_fd_error("21sh: cd: not a directory: ",
				ft_strrchr(path, '/') + 1, NULL, NULL));
		}
		if (access(path, X_OK) == 0)
		{
			gen_oldpwd((tmp = get_cwd()), env_list);
			chdir(path);
			gen_pwd((tmp2 = get_cwd()), env_list);
			ft_strdel_2(&tmp, &tmp2);
		}
		else if (ft_strrchr(path, '/'))
			ft_putendl_fd_error(ERROR5, ft_strrchr(path, '/') + 1, "\n", NULL);
	}
	else if (ft_strrchr(path, '/'))
		ft_putendl_fd_error(ERROR6, ft_strrchr(path, '/') + 1, "\n", NULL);
	else
		ft_putendl_fd_error(ERROR6, path, "\n", NULL);
}

static void		cd_home(t_env **env_list)
{
	char		*home;

	home = NULL;
	if (!(home = get_value_expansion("HOME", env_list)))
		return ;
	cd_simple(home, env_list);
	ft_strdel(&home);
}

static void		cd_back(t_env **env_list)
{
	char		*back_path;

	back_path = NULL;
	if (!(back_path = get_value_expansion("OLDPWD", env_list)))
		return ;
	cd_simple(back_path, env_list);
	ft_strdel(&back_path);
}

int				check_args(char **cmd, t_env **env_list)
{
	if (check_args_no(cmd) > 2)
		ft_putendl_fd_int("21sh: cd: Too many arguments.", 2, 1);
	if (check_args_no(cmd) == 1)
	{
		cd_home(env_list);
		return (1);
	}
	return (0);
}

void			blt_cd(char **cmd, t_env **env_list)
{
	char		*new_path;
	char		*cwd;
	char		buff[MAX_INDEX];

	cwd = NULL;
	if (check_args(cmd, env_list) == 1)
		return ;
	if (check_args_no(cmd) == 2)
	{
		ft_strcpy(buff, cmd[1]);
		if (buff[0] != '/' && buff[0] != '.' && buff[0] != '-')
		{
			if (!(cwd = get_cwd()))
				return ;
			ft_strcat(cwd, "/");
			if (!(new_path = ft_strjoin(cwd, buff)))
				return ;
			cd_simple(new_path, env_list);
			ft_strdel_2(&new_path, &cwd);
		}
		else if (cmd[1][0] == '-' && cmd[1][1] == '\0')
			cd_back(env_list);
		else
			cd_simple(buff, env_list);
	}
}
