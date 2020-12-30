/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 17:51:48 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/23 22:30:13 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

char	*err_ret(char *s, char *addrr)
{
	ft_putstr_fd(s, 2);
	return (addrr);
}

void	error_message(char *err_message, int flag)
{
	ft_putstr_fd(err_message, 2);
	if (flag)
		exit(EXIT_FAILURE);
}
