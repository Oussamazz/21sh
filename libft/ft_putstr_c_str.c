/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_c_str.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 05:25:02 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/23 05:29:04 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_c_str(char *str, char c, char *str2, int fd)
{
	if (str)
		ft_putstr_fd(str, fd);
	ft_putchar_fd(c, fd);
	if (str2)
		ft_putstr_fd(str2, fd);
}
