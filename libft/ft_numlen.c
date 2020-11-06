/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:40:16 by oelazzou          #+#    #+#             */
/*   Updated: 2019/04/06 00:06:31 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int		ft_numlen(int num)
{
	int res;
	int n;

	res = 0;
	if (num == 0)
		return (1);
	else if (num < 0)
	{
		res++;
		n = -num;
	}
	else
		n = num;
	while (n > 0)
	{
		res++;
		n /= 10;
	}
	return (res);
}
