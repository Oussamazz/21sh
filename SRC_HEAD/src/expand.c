/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 22:07:06 by macos             #+#    #+#             */
/*   Updated: 2020/12/08 12:24:12 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "21sh.h"

char        *expanded(t_env **head, char *str)
{
    char **arr;
    char **value;
    char *string;
    char *tmp;
    int i;
    int j;
    size_t val_size;

    arr = NULL;
    value = NULL;
    if(!(arr = strsplit(str)))
        return (NULL);
    if (arr)
    {
        val_size = get_arr_size(arr) + 1;
        string = NULL;
        tmp = NULL;
        if (!(value = (char**)ft_memalloc(sizeof(char *) * val_size)))
            return (NULL);
        j = 0;
        i = 0;
        while (arr[i] != NULL && j < val_size && i < val_size)
        {
            if (ft_is_expansion((tmp = ft_strchr(arr[i], '$'))))
            {
                if (tmp > arr[i] && *(tmp - 1) == '\\')
                {
                    i++;
                    continue ;
                }
                if (!(value[j] = get_value_expansion(ft_strchr(arr[i], '$') + 1, head)))
                {
                    i++;
                    continue ;
                }
                if (string)
                {
                    tmp = string;
                    string = ft_strjoin(string, value[j]);
                    ft_strdel(&tmp);
                }
                else
                    string = value[j];
                j++;
            }
            else
            {
                if (string && !ft_is_expansion(arr[i]))
                {
                    tmp = string;
                    string = ft_strjoin(string, arr[i]);
                    ft_strdel(&tmp);
                }
                if (!i && !j && !ft_is_expansion(arr[i]))
                    string = ft_strdup(arr[i]);
            } 
            i++;
        }
        return (string);
    }
    return (NULL);
}
