/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/01 18:48:56 by oelazzou             #+#    #+#             */
/*   Updated: 2020/12/27 00:56:06 by oelazzou            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    ft_free_arr(char **arr)
{
    int i;

    if (arr)
    {
        i = 0;
        while (arr[i] != NULL)
        {
            ft_strdel(&arr[i]);
            i++;
        }
        if (arr)
            free(arr);
        arr = NULL;
    }
}