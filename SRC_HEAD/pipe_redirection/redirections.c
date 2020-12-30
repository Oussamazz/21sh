/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/23 17:41:00 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/30 18:34:08 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static char		*get_left_fd(char *str, int pos)
{
	int			i;
	int			j;
	char		*tmp;

	tmp = ft_strnew(ft_strlen(str));
	if (!tmp)
	{
		i = pos;
		j = 0;
		while (i >= 0 && str[i] && !is_blank(str[i]))
		{
			tmp[j] = str[i];
			i--;
			j++;
		}
		return (tmp);
	}
	return (NULL);
}

static int		calc_symbol(char *str)
{
	int			count;

	count = 0;
	while (str[count] && ft_is_there(AGG_REDI, str[count]))
		count++;
	return (count);
}

size_t			redirerction_parse(t_lexer **token_node, char **agg,
	t_pointt *cor)
{
	int			j;

	j = 0;
	while (agg[j] != NULL && agg[j][0] != '\0')
	{
		if (ft_isdigit(agg[j][0]) && j == 0)
			append_list_redi(token_node, ft_strdup(agg[j]), L_REDIR, cor);
		else if (ft_is_there(AGG_REDI, agg[j][0]))
			append_list_redi(token_node, ft_strdup(agg[j]), AGGR_SYM, cor);
		else if (ft_isascii(agg[j][0]))
			append_list_redi(token_node, ft_strdup(agg[j]), R_REDIR, cor);
		j++;
	}
	ft_free_arr(agg);
	return (g_agg_len);
}
