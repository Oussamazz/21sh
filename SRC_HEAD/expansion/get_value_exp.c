/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value_exp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 19:04:25 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char		*get_value_expansion(char *env_name, t_env **head)
{
	char	*env_value;
	t_env	*curr;

	curr = NULL;
	env_value = NULL;
	if (head)
		curr = *head;
	if (env_name && curr)
	{
		while (curr)
		{
			if (ft_strequ(curr->env_var_name, env_name))
				return ((env_value = ft_strdup(curr->env_var_value)));
			curr = curr->next;
		}
	}
	return (env_value);
}
