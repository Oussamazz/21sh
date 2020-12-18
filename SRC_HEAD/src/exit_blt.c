/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_blt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:26:11 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/18 19:45:52 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    exit_blt(t_miniast **root, t_lexer **lex, t_env **env_list ,char **buff)
{
    ft_free_tokenz(lex);
    ft_free_tree(root);
    free_env_list(env_list);
    ft_free_history();
    ft_strdel(buff);
}
