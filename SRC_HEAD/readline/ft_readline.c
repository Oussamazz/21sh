/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 00:09:39 by yabakhar          #+#    #+#             */
/*   Updated: 2020/12/21 23:46:49 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

int ft_output(int str)
{
	ft_putchar(str);
	return (0);
}

void cur_goto(t_line *line, int cursor)
{
	int x;
	int y;

	y = (line->c_v.y + (line->c_v.x + cursor) / line->col);
	x = ((line->c_v.x + cursor) % line->col);
	tputs(tgoto(tgetstr("cm", 0), x, y), 0, ft_output);
}

void ft_put_multistring(char **strings)
{
	while (*strings)
		ft_putstr(*strings++);
}

void handle_sigwindch(int sig)
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	if (sig == SIGWINCH)
	{
		g_line->col = w.ws_col;
		g_line->row = w.ws_row;
		tputs(tgoto(tgetstr("cm", 0), 0, g_line->c_o.y), 0, ft_output);
		tputs(tgetstr("cd", 0), 0, ft_output);
		ft_prompte();
		ft_clear(g_line, g_str);
	}
}

void ft_ctl_l(t_line *line, char *str)
{
	tputs(tgoto(tgetstr("cm", 0), 0, 0), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
	ft_prompte();
	line->c_o.y = 0;
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	print_line(str);
	move_cursor_v(line);
	cur_goto(line, line->cursor);
}

void ft_ctl_c(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(0, TIOCSTI, "\12");
		g_clt_c = 1;
	}
}

void ft_signale(void)
{
	signal(SIGWINCH, handle_sigwindch);
	signal(SIGINT, ft_ctl_c);
}

void ft_prompte(void)
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
		ft_put_multistring((char *[]){"\033[1;32m➜ ", "\033[1;36m ", cwd1, " $> \033[0m", 0});
		return;
	}
	str1 = ft_strrchr(cwd1, '/');
	ft_put_multistring((char *[]){"\033[1;32m➜ ", "\033[1;36m ", str1 + 1, " $>\033[0m", 0});
}

void get_cursor_position(t_line *line)
{
	char *buff;
	int i = 0;
	buff = (char[20]){0};
	while (1)
	{
		ft_putstr_fd("\e[6n", 2);
		i = read(2, buff, 20);
		buff[i] = 0;
		if (ft_strchr(buff, '['))
			break;
	}
	line->c_o.y = ft_atoi(buff + 2) - 1;
	if ((buff = (char *)ft_strchr(buff, ';')))
		line->c_o.x = ft_atoi(buff + 1);
	else
		line->c_o.x = 0;
	line->c_v = line->c_o;
}

void ft_set_terminal(void)
{
    struct termios config;
	
	char buf[1024];
    if (tcgetattr(0, &config) < 0)
        ft_putendl_fd("error",2);
    config.c_lflag &= ~(ECHO | ICANON);
    if (tcsetattr(0, 0, &config) < 0)
        ft_putendl_fd("error",2);
	tgetent(buf, getenv("TERM"));
}

void ft_unset_terminal(void)
{
    struct termios config;

    if (tcgetattr(0, &config) < 0)
        ft_putendl_fd("error",2);
    config.c_lflag |= (ECHO | ICANON);
    if (tcsetattr(0, 0, &config) < 0)
        ft_putendl_fd("error",2);
}

void ft_init(t_line *line, t_node **current)
{
    struct winsize w;

    ft_set_terminal();
    ioctl(0, TIOCGWINSZ, &w);
    ft_bzero(line, sizeof(t_line));
    line->col = w.ws_col;
    line->row = w.ws_row;
	g_clt_c = 0;
	g_clt_D = 0;
    get_cursor_position(line);
    tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
    *current = add_to_history("");
    ft_history_goto(current, *current, line);
}

void print_line(char *str)
{
	while (str && *str)
	{
		if (*str == '\n')
			ft_putchar(' ');
		ft_putchar(*str);
		str++;
	}
	ft_putchar(' ');
}

void ft_clear(t_line *line, char *str)
{
	tputs(tgoto(tgetstr("cm", 0), line->c_o.x, line->c_o.y), 0, ft_output);
	tputs(tgetstr("cd", 0), 0, ft_output);
	print_line(str);
	ft_update_cursor_o(line);
	cur_goto(line, line->cursor);
}

void ft_print_print(char **str, t_line *line, char *buff)
{
	int i;

	i = -1;
	while (buff[++i])
		if (ft_isprint(buff[i]) || buff[i] == '\n')
			ft_printnbl(str, line, buff[i]);
	ft_clear(line, *str);
}

int  keyshendle(t_line *line, char **str)
{
	int r = 0;
	if (line->r == LEFT && (r = 1))
		move_left(line, *str);
	else if (line->r == RIGHT && (r = 1))
		move_right(line, *str);
	else if (line->r == ALT_S && (r = 1))
		ft_chack_selction(line, *str);
	else if (line->r == ALT_V && line->sltstr && (r = 1))
	{
		line->slct = 0;
		line->slctd = 0;
		line->slctf = 0;
		ft_print_print(str, line, line->sltstr);
	}
	else if (line->r == ALT_C && (r = 1))
		ft_copie(line, *str);
	else if (line->r == PAGE_DOWN && line->slct == 0 && (r = 1))
		move_down(line);
	else if (line->r == PAGE_UP && line->slct == 0 && (r = 1))
		move_up(line);
	return (r);
}

void ft_free_history(void)
{
	t_node *new;
	
	new = add_to_history(NULL);
	while (new)
	{
		ft_strdel(&(new->content));
		ft_strdel(&(new->tmp));
		new = new->next;
	}
}

int keyshendle2(t_line *line, char **str,int flag)
{
	int r = 0;
	if (line->r == ALT_RTH && line->slct == 0 && (r = 1))
		ft_alt_rth(*str, line);
	else if (line->r == ALT_LFT && line->slct == 0 && (r = 1))
		ft_alt_lft(*str, line);
	else if (line->r == DEL && line->slct == 0 && (r = 1))
		ft_delet(str, line);
	else if (line->r == DELETE && line->slct == 0 && (r = 1))
		ft_delet(str, line);
	else if (line->r == CTRL_L && line->slct == 0 && (r = 1))
		ft_ctl_l(line, *str);
	else if (line->r == ALT_D && (!line->b_line) && line->slct == 0)
	{
		if (!flag)
		{
			ft_free_history();
			exit(0);
		}
		else
		{
			ioctl(0, TIOCSTI, "\12");
			g_clt_D = 1;
		}
	}
	return (r);
}

int keyshendle1(t_line *line, char **str, t_node **current)
{
	int r;
	
	r = 0;
	if ((line->r == HOME || line->r == DEEP) && line->slct == 0 && (r = 1))
		home_deep(line, *str);
	else if (line->r == UP && line->slct == 0 && (r = 1))
		ft_history_goto(current, (*current)->next, line);
	else if (line->r == DOWN  && line->slct == 0 && (r = 1))
		ft_history_goto(current, (*current)->prev, line);
	return (r);
}

char *ft_readline(int flag)
{
	t_node *current;
	char buff[MAX_INDEX];
	t_line  line;
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
			if (keyshendle(&(line),&current->tmp))
				continue ;
			else if (keyshendle1(&line, &current->tmp, &current))
				continue ;
			else if (keyshendle2(&(line), &current->tmp, flag))
				continue ;
			else if ((line.r == END && line.slct == 0 ) || g_clt_c == 1)
				break ;
			else if (line.slct == 0)
				ft_print_print(&current->tmp, &(line), buff);
		}
	}
	return (ft_end(&current, &line));
}