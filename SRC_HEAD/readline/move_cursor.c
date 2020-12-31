/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/11 16:19:30 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	move_right(t_line *line, char *str)
{
	if (line->slct)
	{
		if (line->slctd > line->c_len)
			ft_unselect(line, str);
		else if (line->slctd <= line->c_len)
			ft_select(line, str);
	}
	if (line->cursor < line->len - (line->i != line->index))
	{
		line->cursor++;
		cur_goto(line, line->cursor);
		line->c_len++;
	}
	else if (line->cursor == line->len - 1 &&
		line->index > 0 && line->index > line->i)
	{
		line->i++;
		line->len = line->tabl[line->i];
		move_cursor_v(line);
		line->cursor = 0;
		cur_goto(line, line->cursor);
		line->c_len++;
	}
}

void	move_left(t_line *line, char *str)
{
	if (line->slct && line->cursor > 0)
	{
		if (line->slctd >= line->c_len)
			ft_select(line, str);
		else if (line->slctd < line->c_len)
			ft_unselect(line, str);
	}
	if (line->cursor > 0)
	{
		line->cursor--;
		cur_goto(line, line->cursor);
		line->c_len--;
	}
	else if (line->cursor == 0 && line->index > 0 && line->i > 0)
	{
		line->i--;
		line->len = line->tabl[line->i];
		line->cursor = line->len - 1;
		move_cursor_v(line);
		cur_goto(line, line->cursor);
		line->c_len--;
	}
}

void	move_up(t_line *line)
{
	if (line->i > 0)
	{
		line->i--;
		line->len = line->tabl[line->i];
		move_cursor_v(line);
		if (line->cursor > line->len - 1)
			line->cursor = line->len - 1;
		if (line->i == 0)
			tputs(tgoto(tgetstr("cm", 0), line->c_o.x + line->cursor,
				line->c_v.y), 0, ft_output);
		else
			tputs(tgoto(tgetstr("cm", 0),
				line->cursor, line->c_v.y), 0, ft_output);
		line->c_len = (count_len(line) + line->cursor);
	}
}

void	move_down(t_line *line)
{
	if (line->i < line->index)
	{
		line->i++;
		line->len = line->tabl[line->i];
		move_cursor_v(line);
		if (line->cursor > line->len - 1)
			line->cursor = line->len - 1;
		tputs(tgoto(tgetstr("cm", 0), line->cursor,
			line->c_v.y), 0, ft_output);
		line->c_len = (count_len(line) + line->cursor);
	}
}

void	home_deep(t_line *line, char *str)
{
	if (str && line->r == HOME)
	{
		line->c_v = line->c_o;
		line->i = 0;
		line->c_len = 0;
		line->cursor = 0;
		line->len = line->tabl[line->i];
		tputs(tgoto(tgetstr("cm", 0), line->c_v.x, line->c_v.y), 0, ft_output);
	}
	else if (str && line->r == DEEP)
	{
		line->b_line = ft_strlen(str);
		line->i = line->index;
		line->c_len = line->b_line;
		line->len = line->tabl[line->i];
		line->cursor = line->len;
		move_cursor_v(line);
		tputs(tgoto(tgetstr("cm", 0), (line->c_v.x + line->cursor) % line->col,
					(line->c_v.y + (line->c_v.x + line->cursor) / line->col)),
					0, ft_output);
	}
}
