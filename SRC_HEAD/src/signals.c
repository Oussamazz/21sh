/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:53:13 by oelazzou          #+#    #+#             */
/*   Updated: 2021/02/15 18:55:36 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	ft_ctrlc(int sig_no)
{
	ft_putstr_fd("shloop sighander\n", 1);
	(void)sig_no;
	ft_putchar_fd('\n', 1);
	ft_prompte();
}

char	*handel_signal(t_getfullcmd *v)
{
	if (g_clt_d)
	{
		ft_putstr_fd("unexpected EOF while looking for matching `", 2);
		ft_putchar_fd(v->c, 2);
		ft_putendl_fd("\"", 2);
	}
	ft_strdel(&v->cmd);
	return (NULL);
}

void	sig_groupe(void)
{
	int fd = 0;

	if ((fd = open(ttyname(0), O_RDWR)) == -1)
		return ;
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	if (tcsetpgrp(fd, getpid()) == -1)
		printf("somt went wrong\n");
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	close(fd);
}
