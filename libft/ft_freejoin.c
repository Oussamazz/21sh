/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freejoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabakhar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 18:25:54 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/28 18:30:51 by yabakhar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_freejoin(char *s1, char *s2, int num)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	if (num == 0)
		free(s1);
	else if (num == 1)
		free(s2);
	else if (num == 2)
	{
		free(s1);
		free(s2);
	}
	return (str);
}
