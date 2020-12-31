/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_alt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 23:33:30 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	alt_cgoto(t_line *line)
{
	int i;
	int nb;

	nb = 0;
	i = 0;
	while (i <= line->index)
	{
		nb += line->tabl[i];
		if (nb > line->c_len)
			break ;
		i++;
	}
	if (nb > line->c_len)
	{
		line->i = i;
		line->cursor = (line->tabl[i] - (nb - line->c_len));
	}
}

int		count_row(t_line *line)
{
	int i;
	int y;

	i = 0;
	y = (line->c_o.x + line->tabl[0] + (line->index == i)) % line->col > 0;
	y += (line->c_o.x + line->tabl[0] + (line->index == i)) / line->col;
	while (++i <= line->index && line->tabl)
	{
		y += ((line->tabl[i] + (line->index == i)) % line->col) > 0;
		y += ((line->tabl[i] + (line->index == i)) / line->col);
	}
	return (y);
}

void	ft_update_cursor_o(t_line *line)
{
	if (line->tabl && (line->c_o.y + count_row(line) - line->row > 0))
		line->c_o.y -= ((line->c_o.y + count_row(line) - line->row));
	move_cursor_v(line);
}

void	ft_alt_rth(char *str, t_line *line)
{
	if (line->tabl && line->cursor < line->tabl[line->i])
	{
		while (str[line->c_len])
		{
			if (!ft_isalnum(str[line->c_len]))
				break ;
			line->cursor++;
			line->c_len++;
		}
		while (str[line->c_len])
		{
			if (ft_isalnum(str[line->c_len]))
				break ;
			line->cursor++;
			line->c_len++;
		}
		alt_cgoto(line);
		move_cursor_v(line);
		cur_goto(line, line->cursor);
	}
}

void	ft_alt_lft(char *str, t_line *line)
{
	if (line->tabl)
	{
		while (line->c_len > 0 && str[--line->c_len])
		{
			line->cursor--;
			if (ft_isalnum(str[line->c_len]))
				break ;
		}
		while (line->c_len > 0 && str[--line->c_len])
		{
			line->cursor--;
			if (!ft_isalnum(str[line->c_len]))
				break ;
		}
		if (line->c_len > 0)
		{
			line->cursor++;
			line->c_len++;
		}
		alt_cgoto(line);
		move_cursor_v(line);
		cur_goto(line, line->cursor);
	}
}
