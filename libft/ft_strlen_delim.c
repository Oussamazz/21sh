/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_delim.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 14:32:19 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/26 14:32:32 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlen_delim(char *s, char *delim)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0' && !ft_is_there(delim, s[len]))
		len++;
	return (len);
}
