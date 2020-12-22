/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_blt.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:26:11 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/22 18:33:23 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void    exit_blt(t_miniast **root, t_lexer **lex, t_env **env_list ,char **buff)
{
    ft_free_history();
    if (lex)
        ft_free_tokenz(lex);
    if (root)
        ft_free_tree(root);
    if (env_list)
        free_env_list(env_list);
    if (buff)
        ft_strdel(buff);
}
