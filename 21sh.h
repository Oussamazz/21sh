/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 12:55:33 by yabakhar          #+#    #+#             */
/*   Updated: 2020/11/06 12:35:45 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <curses.h>
#include <term.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "libft/libft.h"

#define UP 4283163
#define DOWN 4348699
#define LEFT 4479771
#define RIGHT 4414235
#define SPACE 32
#define DEL 127
#define END 10
#define ESC 27
#define alt_rth 1130044187
#define alt_lft 1146821403
#define HOME 4741915
#define DEEP 4610843
#define alt_C 42947
#define alt_V 10127586
#define alt_S 40899
#define page_down 1113266971
#define page_up 1096489755
#define ALT_D 4
#define ALT_S 40899
#define ALT_V 10127586
#define TRUE 1
#define ALT_C 42947
#define CTRL_L 12
#define DELETE 2117294875

typedef struct s_init
{
	int k;
	int r;
	char c[2];
} t_init;

typedef struct s_node
{
	char *content;
	char *content_len;
	char *tmp;
	int len;
	struct s_node *next;
	struct s_node *prev;
} t_node;

typedef struct s_point
{
	int x;
	int y;
} t_point;

typedef struct s_line
{
	t_point c_o;
	t_point c_v;
	int col;
	int row;
	int len;
	int index;
	int i;
	int cursor;
	int *tabl;
	int c_len;
	int b_line;
	int slct;
	int slctd;
	int slctf;
	char *sltstr;
	int k;
	int r;
	char c[2];
} t_line;

t_line *g_line;
char *g_str;
void ft_stock(char *str, t_node **list, t_line *line);
int ft_output(int str);
void cur_goto(t_line *line, int cursor);
void ft_alt_rth(char *str, t_line *line);
void ft_alt_lft(char *str, t_line *line);
void ft_printnbl(char **str, t_line *line, char c);
void ft_delet(char **str, t_line *line);
void ft_next(t_node **head, t_node **list, char **str, t_line *line);
void ft_prev(t_node **head, t_node **list, char **str, t_line *line);
void get_cursor_position(t_line *line);
void ft_end(t_node **list, t_node **head, t_line *line, char **str);
void print_porompte(t_line *line);
void move_left(t_line *line, char *str);
void move_right(t_line *line, char *str);
void home_deep(t_line *line, char *str);
void esc(void);
void esc1(void);
void ft_init(t_line *line);
void ft_multi(char **str, t_line *line, char **tmp);
void multilne(char *str, t_line *line);
void move_up(t_line *line);
void move_down(t_line *line);
void move_cursor_v(t_line *line);
int count_len(t_line *line);
int count_row(t_line *line);
void ft_update_cursor_o(t_line *line);
void print_line(char *str);
void ft_select(t_line *line, char *str);
void ft_unselect(t_line *line, char *str);
void ft_chack_selction(t_line *line, char *str);
void ft_print_print(char **str, t_line *line, char *buff);
void ft_unselect(t_line *line, char *str);
void ft_clear(t_line *line, char *str);
void ft_copie(t_line *line, char *str);
void ft_porompte(void);
void koko(char **str, t_line *line);
