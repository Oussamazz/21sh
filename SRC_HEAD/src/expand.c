/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 22:07:06 by macos             #+#    #+#             */
/*   Updated: 2020/11/21 17:56:49 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "21sh.h"

char        *expanded(t_env **head, char *str)
{
    char **arr;
    char **value;
    char *string;
    char *tmp;
    char *completion;
    int i;
    int j;
    int o;
    size_t val_size;

    arr = NULL;
    value = NULL;
    completion = NULL;
    arr = strsplit(str);
    if (arr)
    {
        val_size = get_arr_size(arr);
        string = NULL;
        tmp = NULL;
        if (!(value = (char**)ft_memalloc(sizeof(char *) * val_size + 1)))
            return (NULL);
        j = 0;
        i = 0;
        while (arr[i] && j < val_size)
        {
            completion = NULL;
            if (ft_is_expansion(ft_strchr(arr[i], '$')))
            {
                if (ft_strchr(arr[i], '$'))
                    completion = ft_strsub(arr[i], 0, ft_strlen_char(arr[i], '$'));
                if (!(value[j] = get_value_expansion(ft_strchr(arr[i], '$') + 1, head)))
                {
                    i++;
                    continue ;
                }
                if (completion && value[j])
                {
                    tmp = value[j];
                    value[j] = ft_strjoin(completion, value[j]);
                    ft_strdel(&tmp);
                    ft_strdel(&completion);
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
                tmp = string;
                if (!i && !j && !ft_is_expansion(arr[i]))
                    string = ft_strdup(arr[i]);
                else if (j > 0 && arr[i] && value[j - 1] && !ft_is_expansion(arr[i]))
                    string = ft_strjoin(string, arr[i]);
                if (tmp && j)
                    ft_strdel(&tmp);

            } 
            i++;
        }
        return (string);
    }
    return (NULL);
}
