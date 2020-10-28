/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 19:05:25 by oelazzou          #+#    #+#             */
/*   Updated: 2019/09/23 15:44:19 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		line_copy(char **line, char *arr, char c)
{
	int len;

	len = 0;
	while (arr[len] != '\0' && arr[len] != c)
		len++;
	if (!(*line = ft_strndup(arr, len)))
		return (0);
	return (len);
}

int		my_read(int fd, char **arr)
{
	int		res;
	char	buff[BUFF_SIZE + 1];
	char	*tmp;

	while ((res = read(fd, buff, BUFF_SIZE)))
	{
		buff[res] = '\0';
		tmp = *arr;
		*arr = ft_strjoin(*arr, buff);
		if (*arr == NULL)
			return (-1);
		free(tmp);
		if (ft_is_there(buff, '\n'))
			return (res);
	}
	return (res);
}

t_list	*get_valid(int fd, t_list **origin)
{
	t_list *ret;

	if (!origin)
		return (NULL);
	ret = *origin;
	while (ret != NULL)
	{
		if ((int)(ret->content_size) == fd)
			return (ret);
		ret = ret->next;
	}
	ret = ft_lstnew("", fd);
	ft_lstadd(origin, ret);
	return (ret);
}

int		get_next_line(const int fd, char **line)
{
	static	t_list	*origin;
	int				read_res;
	char			buff[BUFF_SIZE + 1];
	t_list			*valid;
	char			*arr;

	if (!line || read(fd, buff, 0) < 0 || !(valid = get_valid(fd, &origin)))
		return (-1);
	arr = valid->content;
	read_res = my_read(fd, &arr);
	valid->content = arr;
	if (!read_res && !*arr)
		return (0);
	read_res = line_copy(line, arr, '\n');
	if (arr[read_res] != '\0')
	{
		valid->content = ft_strdup(arr + read_res + 1);
		ft_strdel(&arr);
	}
	else if (arr[read_res] == '\0')
		ft_strclr(arr);
	return (1);
}
