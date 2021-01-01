/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/21 23:59:46 by yabakhar          #+#    #+#             */
/*   Updated: 2021/01/01 13:21:15 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sh.h"

static t_node *g_history_head;

void	ft_free_history(void)
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

void	ft_history_goto(t_node **current, t_node *new, t_line *line)
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
		ft_multilne((*current)->tmp, line);
		ft_clear(line, (*current)->tmp);
		home_deep(line, (*current)->tmp);
	}
	else
		ft_putstr(tgetstr("bl", NULL));
}

t_node	*add_to_history(const char *str)
{
	t_node *new;

	if (!str)
		return (g_history_head);
	if ((new = (t_node *)ft_memalloc(sizeof(t_node))))
	{
		new->content_len = ft_strlen(str);
		new->content = ft_strdup(str);
		new->next = g_history_head;
		if (g_history_head)
			g_history_head->prev = new;
		g_history_head = new;
		return (new);
	}
	return (g_history_head);
}

void	free_history_node(t_node *head)
{
	if (head)
	{
		if (head->prev == NULL)
			g_history_head = head->next;
		else
			head->prev->next = head->next;
		if (head->next)
			head->next->prev = head->prev;
		free(head->content);
		free(head->tmp);
		free(head);
	}
}

char	*ft_end(t_node **current, t_line *line)
{
	struct termios	config;
	char			*return_line;

	line->r = HOME;
	home_deep(line, (*current)->tmp);
	ft_putendl((*current)->tmp);
	tcgetattr(0, &config);
	config.c_lflag |= (ECHO | ICANON);
	tcsetattr(0, 0, &config);
	return_line = (g_clt_c) ? ft_strdup("") : ft_strdup((*current)->tmp);
	ft_strdel(&(*current)->tmp);
	free_history_node(g_history_head);
	ft_memdel((void **)&(line->tabl));
	if (line->sltstr && *line->sltstr)
		free(line->sltstr);
	return (return_line);
}
