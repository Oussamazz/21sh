/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suspend.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 04:17:08 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/21 04:17:20 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void	suspend(void)
{
	reset_terminal();
	signal(SIGTSTP, SIG_DFL);
	ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
}
