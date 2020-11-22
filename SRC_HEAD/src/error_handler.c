/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macos <macos@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/24 17:51:48 by macos             #+#    #+#             */
/*   Updated: 2020/11/21 11:28:54 by macos            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

char *error_ret(char *addr, char *err_message)
{
    
    return (NULL);
}

void  error_message(char *err_message, int flag)
{
   ft_putstr_fd(err_message, 2);
   if (flag)
    exit(EXIT_FAILURE);
}

/*


if(dup2(input_fds, STDIN_FILENO) < 0) {
  printf("Unable to duplicate file descriptor.");
  exit(EXIT_FAILURE);
}
*/