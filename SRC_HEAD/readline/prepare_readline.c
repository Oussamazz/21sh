/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_readline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/28 17:01:53 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

void	ft_prompte(void)
{
	char cwd[256];
	char *cwd1;
	char *str1;

	if (!getcwd(cwd, sizeof(cwd)))
		return (ft_putstr_fd("$>", 1));
	if (getcwd(cwd, sizeof(cwd)))
		cwd1 = getcwd(cwd, sizeof(cwd));
	if (ft_strcmp(cwd1, "/") == 0)
	{
		ft_put_multistring((char *[]){"\033[1;32m➜ ",
			"\033[1;36m ", cwd1, " $> \033[0m", 0});
		return ;
	}
	str1 = ft_strrchr(cwd1, '/');
	ft_put_multistring((char *[]){"\033[1;32m➜ ",
		"\033[1;36m ", str1 + 1, " $>\033[0m", 0});
}

void	get_cursor_position(t_line *line)
{
	char	*buff;
	int		i;

	i = 0;
	buff = (char[20]){0};
	while (1)
	{
		ft_putstr_fd("\e[6n", 1);
		i = read(1, buff, 20);
		buff[i] = 0;
		if (ft_strchr(buff, '['))
			break ;
	}
	line->c_o.y = ft_atoi(buff + 2) - 1;
	if ((buff = (char *)ft_strchr(buff, ';')))
		line->c_o.x = ft_atoi(buff + 1);
	else
		line->c_o.x = 0;
	line->c_v = line->c_o;
}

void	ft_set_terminal(void)
{
	struct termios	config;
	char			buf[1024];

	if (tcgetattr(0, &config) < 0)
		ft_putendl_fd("error", 2);
	config.c_lflag &= ~(ECHO | ICANON);
	if (tcsetattr(0, 0, &config) < 0)
		ft_putendl_fd("error", 2);
	if (!tgetent(buf, getenv("TERM")))
		exit(1);
}

void	ft_init(t_line *line, t_node **current)
{
	struct winsize w;

	ft_set_terminal();
	ioctl(0, TIOCGWINSZ, &w);
	ft_bzero(line, sizeof(t_line));
	line->col = w.ws_col;
	line->row = w.ws_row;
	g_clt_c = 0;
	g_clt_d = 0;
	get_cursor_position(line);
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	*current = add_to_history("");
	ft_history_goto(current, *current, line);
}
