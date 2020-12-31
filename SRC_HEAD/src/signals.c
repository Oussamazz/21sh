/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 15:53:13 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	ft_ctrlc(int sig_no)
{
	sig_no = 0;
	while (wait(NULL) > 0)
		;
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
		ft_strdel(&v->cmd);
	}
	return (NULL);
}
