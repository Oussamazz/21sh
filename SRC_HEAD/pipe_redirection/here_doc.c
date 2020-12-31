/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 15:50:27 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void			prompt_here_doc(void)
{
	ft_put_multistring2((char *[]){"\033[1;31m>>", "\033[0m", 0, 0, 0, 0, 0});
}

static char		*sig_handler(t_here_doc *v)
{
	if (g_clt_d)
		return (v->text);
	else if (v->text)
		ft_strdel(&v->text);
	return (NULL);
}

static char		*ft_strdel_ret(t_here_doc *v)
{
	ft_strdel(&v->buffer);
	return (v->text);
}

static void		dup_del(t_here_doc *v)
{
	v->text = ft_strdup(v->buffer);
	v->flag = 1;
	ft_strdel(&v->buffer);
}

char			*here_doc(char *delim)
{
	t_here_doc	v;

	ft_bzero(&v, sizeof(t_here_doc));
	while (delim)
	{
		if (g_clt_c || g_clt_d)
			return (sig_handler(&v));
		prompt_here_doc();
		if (!(v.buffer = ft_readline(1)))
			return (NULL);
		if (ft_strequ(v.buffer, delim))
			return (ft_strdel_ret(&v));
		v.buffer = ft_freejoin(v.buffer, "\n", 0);
		v.tmp = v.text;
		if (!v.flag && v.buffer)
		{
			dup_del(&v);
			continue;
		}
		v.text = ft_strjoin(v.text, v.buffer);
		if (v.flag && v.tmp)
			ft_strdel(&v.tmp);
		ft_strdel(&v.buffer);
	}
	return (v.text);
}
