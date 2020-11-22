/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_alt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 23:33:30 by yabakhar          #+#    #+#             */
/*   Updated: 2020/11/21 19:43:25 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

void alt_cgoto(t_line *line)
{
	int i;
	int nb;

	nb = 0;
	i = 0;
	while (i <= line->index)
	{
		nb += line->tabl[i];
		if (nb > line->c_len)
			break;
		i++;
	}
	if (nb > line->c_len)
	{
		line->i = i;
		line->cursor = (line->tabl[i] - (nb - line->c_len));
	}
}

int count_row(t_line *line)
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

void ft_update_cursor_o(t_line *line)
{
	if (line->tabl && (line->c_o.y + count_row(line) - line->row > 0))
		line->c_o.y -= ((line->c_o.y + count_row(line) - line->row));
	move_cursor_v(line);
}

void ft_alt_rth(char *str, t_line *line)
{
	if (line->tabl && line->cursor < line->tabl[line->i])
	{
		while (str[line->c_len])
		{
			if (!ft_isalnum(str[line->c_len]))
				break;
			line->cursor++;
			line->c_len++;
		}
		while (str[line->c_len])
		{
			if (ft_isalnum(str[line->c_len]))
				break;
			line->cursor++;
			line->c_len++;
		}
		alt_cgoto(line);
		move_cursor_v(line);
		cur_goto(line, line->cursor);
	}
}

void ft_alt_lft(char *str, t_line *line)
{
	if (line->tabl)
	{
		while (line->c_len > 0 && str[--line->c_len])
		{
			line->cursor--;
			if (ft_isalnum(str[line->c_len]))
				break;
		}
		while (line->c_len > 0 && str[--line->c_len])
		{
			line->cursor--;
			if (!ft_isalnum(str[line->c_len]))
				break;
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

void ft_delet(char **str, t_line *line)
{
	char *tmp;
	char *tmp1;
	if (line->c_len > 0)
	{
		if ((*str)[line->c_len - 1] == '\n')
		{
			line->i--;
			line->cursor = line->tabl[line->i];
		}
		line->cursor--;
		tmp = ft_strsub(*str, 0, line->c_len - 1);
		tmp1 = ft_strsub(*str, line->c_len, line->b_line - line->c_len);
		ft_strdel(str);
		*str = ft_strjoin(tmp, tmp1);
		ft_strdel(&tmp);
		ft_strdel(&tmp1);
		tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		print_line(*str);
		line->b_line--;
		line->len--;
		line->c_len--;
		ft_multilne(*str, line);
		move_cursor_v(line);
		cur_goto(line, line->cursor);
	}
}

void ft_print(char **str, char *c, int c_len, t_line *line)
{
	char *tmp;
	char *tmp1;
	char *tmp2;

	tmp = ft_strsub(*str, 0, c_len);
	tmp1 = ft_strsub(*str, c_len, line->b_line - c_len);
	ft_strdel(str);
	tmp2 = ft_strjoin(tmp, c);
	*str = ft_strjoin(tmp2, tmp1);
	ft_strdel(&tmp2);
	ft_strdel(&tmp1);
	ft_strdel(&tmp);
}

void ft_printnbl(char **str, t_line *line, char c)
{
	ft_strcpy(line->c, (char[]){c, 0});
	if (!(*str))
		(*str) = ft_strdup("");
	ft_print(str, line->c, line->c_len, line);
	line->len++;
	if (c == '\n')
	{
		line->i++;
		line->cursor = 0;
	}
	else
		line->cursor++;
	line->b_line++;
	ft_multilne(*str, line);
	line->c_len++;
}
