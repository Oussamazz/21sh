/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_four.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 11:03:18 by oelazzou          #+#    #+#             */
/*   Updated: 2019/10/06 11:14:12 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_four(char const *s1, char const *s2,
char const *s3, char const *s4)
{
	char *ret;
	char *tmp1;
	char *tmp2;

	ret = NULL;
	if (s1 && s2 && s3 && s4)
	{
		tmp1 = ft_strjoin(s1, s2);
		tmp2 = ft_strjoin(s3, s4);
		ret = ft_strjoin(tmp1, tmp2);
		ft_strdel(&tmp1);
		ft_strdel(&tmp2);
	}
	return (ret);
}
