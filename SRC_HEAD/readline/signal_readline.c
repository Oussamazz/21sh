/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 14:41:42 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	ft_ctl_d(int flag)
{
	if (!flag)
	{
		ft_free_history();
		exit(0);
	}
	else
	{
		ioctl(0, TIOCSTI, "\12");
		g_clt_d = 1;
	}
}

void	handle_sigwindch(int sig)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	if (sig == SIGWINCH)
	{
		g_line->col = w.ws_col;
		g_line->row = w.ws_row;
		tputs(tgoto(tgetstr("cm", 0), 0, g_line->c_o.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		ft_prompte();
		ft_clear(g_line, g_str);
	}
}

void	ft_ctl_c(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(0, TIOCSTI, "\12");
		g_clt_c = 1;
	}
}

void	ft_signale(void)
{
	signal(SIGWINCH, handle_sigwindch);
	signal(SIGINT, ft_ctl_c);
}
