/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 00:09:39 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

int			ft_output(int str)
{
	ft_putchar(str);
	return (0);
}

void		ft_delet(char **str, t_line *line)
{
	char	*tmp;
	char	*tmp1;

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
		*str = ft_freejoin(tmp, tmp1, 2);
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

void		ft_print(char **str, char *c, int c_len, t_line *line)
{
	char	*tmp;
	char	*tmp1;
	char	*tmp2;

	tmp = ft_strsub(*str, 0, c_len);
	tmp1 = ft_strsub(*str, c_len, line->b_line - c_len);
	ft_strdel(str);
	tmp2 = ft_strjoin(tmp, c);
	*str = ft_strjoin(tmp2, tmp1);
	ft_strdel(&tmp2);
	ft_strdel(&tmp1);
	ft_strdel(&tmp);
}

void		ft_printnbl(char **str, t_line *line, char c)
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

char		*ft_readline(int flag)
{
	char	buff[MAX_INDEX];
	t_node	*current;
	t_line	line;

	ft_init(&(line), &current);
	while (TRUE)
	{
		ft_signale();
		g_line = &line;
		g_str = current->tmp;
		line.r = 0;
		ft_bzero(buff, 1024);
		if (read(0, buff, 1023) > 0)
		{
			line.r = (*(int *)buff);
			if (!ft_readline_builtines(flag, buff, &line, &current))
				break ;
		}
	}
	return (ft_end(&current, &line));
}
