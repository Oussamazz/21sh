/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 16:03:27 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/01 16:29:51 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

void	ft_putnbr(int n)
{
	unsigned int nb;

	if (n < 0)
	{
		ft_putchar('-');
		nb = -n;
	}
	else
		nb = n;
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putchar(nb % 10 + 48);
	}
	if (nb < 10)
		ft_putchar(nb + 48);
}
