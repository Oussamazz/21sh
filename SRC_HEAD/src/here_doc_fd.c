/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelazzou <oelazzou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/31 16:17:05 by oelazzou          #+#    #+#             */
/*   Updated: 2020/12/31 18:41:55 by oelazzou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int			here_document(t_redir *redirection, char *tty_name)
{
	int		pip[2];
	int		fd;
	int		tmp;

	tmp = 255;
	if ((fd = open(tty_name, O_RDWR)) == -1)
		return (-1);
	dup2(fd, STDIN_FILENO);
	dup2(STDOUT_FILENO, tmp);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if ((pipe(pip)) == -1)
		return (-2);
	if (redirection->next && redirection->next->rfd)
		ft_putstr_fd(redirection->next->rfd, pip[1]);
	else if (redirection->next == NULL)
		return (ft_putendl_fd_int("21sh: parse error near `\\n'", 2, -3));
	close(pip[1]);
	dup2(pip[0], STDIN_FILENO);
	close(pip[0]);
	dup2(tmp, STDOUT_FILENO);
	close(tmp);
	return (255);
}
