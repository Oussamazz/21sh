/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/21 23:59:46 by yabakhar          #+#    #+#             */
/*   Updated: 2020/05/22 03:21:16 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_21sh.h"

static t_node *history_head;

void ft_history_goto(t_node **current, t_node *new, t_line *line)
{
	if (new)
	{
		if (ft_strequ((*current)->content, (*current)->tmp))
			ft_strdel(&(*current)->tmp);
		*current = new;
		if ((*current)->tmp == NULL)
		{
			(*current)->tmp = ft_strdup((*current)->content);
			(*current)->len = (*current)->content_len;
		}
		line->r = DEEP;
		multilne((*current)->tmp, line);
		ft_clear(line, (*current)->tmp);
		home_deep(line, (*current)->tmp);
	}
	else
		ft_putstr(tgetstr("bl", NULL));
}

void load_hsitory(const char *file)
{
	char *line;

	line = NULL;
	int fd = open(file, O_RDONLY);
	while (get_next_line(fd, &line) > 0)
	{
		add_to_history(line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	close(fd);
}

t_node *add_to_history(const char *str)
{
	t_node *new;

	if ((new = (t_node *)ft_memalloc(sizeof(t_node))))
	{
		new->content_len = ft_strlen(str);
		new->content = ft_strdup(str);
		new->next = history_head;
		if (history_head)
			history_head->prev = new;
		history_head = new;
		return (new);
	}
	return (history_head);
}

void free_history_node(t_node *node)
{
	if (node)
	{
		if (node->prev == NULL)
			history_head = node->next;
		else
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
		free(node->content);
		free(node->tmp);
		free(node);
	}
}

char *ft_end(t_node **current, t_line *line)
{
	char *return_line;
	struct termios config;

	line->r = HOME;
	home_deep(line, (*current)->tmp);
	ft_putendl((*current)->tmp);
	tcgetattr(0, &config);
	config.c_lflag &= (ECHO | ICANON);
	tcsetattr(0, 0, &config);

	return_line = ft_strdup((*current)->tmp);
	ft_strdel(&(*current)->tmp);
	free_history_node(history_head);
	return return_line;
}