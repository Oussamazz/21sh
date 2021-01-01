/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arr_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 11:31:43 by oelazzou          #+#    #+#             */
/*   Updated: 2020/11/18 11:33:42 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		get_arr_size(char **arr)
{
	size_t	size;

	size = -1;
	if (arr && *arr)
	{
		size++;
		while (arr[size])
			size++;
	}
	return (size);
}
