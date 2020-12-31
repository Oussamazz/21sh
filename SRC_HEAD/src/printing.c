/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 22:19:31 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 19:04:56 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		ft_putendl_fd_error(char *s1, char *s2, char *s3, char *s4)
{
	if (s1)
		ft_putstr_fd(s1, STDERR_FILENO);
	if (s2)
		ft_putstr_fd(s2, STDERR_FILENO);
	if (s3)
		ft_putstr_fd(s3, STDERR_FILENO);
	if (s4)
		ft_putstr_fd(s4, STDERR_FILENO);
	return ;
}

void		ft_put_multistring2(char **strings)
{
	while (*strings)
		ft_putstr(*strings++);
}

void		prompt_completion(char quot)
{
	if (quot == '\"')
		ft_put_multistring2((char *[]){"\033[1;31m\"", "\033[0m",
			"\033[1;32m>", "\033[0m", 0, 0, 0});
	else
		ft_put_multistring2((char *[]){"\033[1;31m\'", "\033[0m",
			"\033[1;32m>", "\033[0m", 0, 0, 0});
	return ;
}

static int	check_if(char **av)
{
	if (ft_is_there(av[1], 'g') || ft_is_there(av[2], 'G'))
		return (1);
	return (0);
}

void		starting_message(char **argv, char **user, time_t *now)
{
	if (check_if(argv) && *user)
	{
		ft_putstr_fd("\e[1;1H\e[2J", 1);
		if (*user)
		{
			ft_putstr_fd(*user, 1);
			ft_strdel(user);
			ft_putstr_fd(" logged, ", 1);
			ft_putstr_fd(ttyname(0), 1);
		}
		ft_putstr_fd(WELCOME_MSG0, 1);
		ft_putstr_fd(WELCOME_MSG1, 1);
		ft_putendl_fd(ctime(now), 1);
	}
}
