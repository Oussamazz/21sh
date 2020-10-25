#include "../includes/ft_21sh.h"

int main(int argc, char **argv, char **envp)
{
	char	*tmp;
	char	*line;
	t_exec	*head;

    argc = 0;
	argv = NULL;
	g_env.list = NULL;
	g_env.head = g_env.list;
	ft_set_input_mode();
	if (tgetent(NULL, (getenv("TERM")) ? getenv("TERM") : TERM) > 0)
	{
		ft_signale();
		ft_init_env(envp);
		while (TRUE)
		{
			ft_porompte();
			tmp = ft_readline();
			line = (tmp) ? ft_strtrim(tmp) : NULL;
			if (tmp && *line)
			{
				ft_parse(&line);
				ft_parse_command_line(&head, line);
				ft_execute(head);
				ft_free_line(&head);
			}
		}
	// 	ft_free_env();
	}
	return (0);
}