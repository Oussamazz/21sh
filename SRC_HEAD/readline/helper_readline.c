/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 14:23:55 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

int			count_len(t_line *line)
{
	int		i;
	int		k;

	k = 0;
	i = line->i;
	while (i > 0)
	{
		i--;
		k += line->tabl[i];
	}
	return (k);
}

void		ft_stock_totable(t_line *line, char *str)
{
	int		k;
	int		i;

	k = 0;
	i = 0;
	while (str && str[i])
	{
		line->tabl[k]++;
		if (str[i] == '\n')
			k++;
		i++;
	}
}

void		ft_allocate_table(t_line *line, char *str)
{
	int		i;

	i = 0;
	if (str)
	{
		while (str[i])
		{
			if (str[i] == '\n')
				line->index++;
			i++;
		}
	}
	if (line->tabl)
		ft_memdel((void **)&line->tabl);
	line->tabl = ft_memalloc(sizeof(int) * (line->index + 1));
	ft_stock_totable(line, str);
}

void		ft_multilne(char *str, t_line *line)
{
	line->len = 0;
	line->index = 0;
	ft_allocate_table(line, str);
	line->len = line->tabl[line->i];
}

void		move_cursor_v(t_line *line)
{
	t_point point;
	int		i;

	point = line->c_o;
	i = line->i;
	while (i > 0)
	{
		i--;
		if (i == 0)
		{
			point.y += ((line->tabl[i] + line->c_o.x) / line->col);
			if (((line->tabl[i] + line->c_o.x) % line->col) > 0)
				point.y += 1;
		}
		else
		{
			point.y += ((line->tabl[i]) / line->col);
			if (((line->tabl[i]) % line->col) > 0)
				point.y += 1;
		}
		point.x = 0;
	}
	line->c_v = point;
}
