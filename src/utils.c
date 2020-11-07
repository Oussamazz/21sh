
#include "21sh.h"

bool    is_quote(int c)
{
    if (c == '\'' || c == '\"')
        return (true);
    return (false);
}

size_t last_node_check(t_lexer **tokenz, t_type type, int node_index)
{
    t_lexer *head;

    if (tokenz && node_index)
    {
        if (!*tokenz)
            return (0);
        head = *tokenz;
        while (head->next != NULL)
        {
            if (head->aggr_index)
            {
                head->aggr_index = node_index;
                break ;
            }
            head = head->next;
        }
        if (head->type == type)
            return (1);
        return (0);
    }
    return (0);
}

size_t ft_strlen_char(char *s, char c)
{
    size_t len;

    len = 0;
    while (s[len] != '\0' && s[len] != c)
        len++;
    return (len);
}

char	*ft_strchr_blank(const char *str, int c)
{
    int i;
    char *s;

    i = 0;
    s = (char*)str;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (&s[i]);
        else if (is_blank(s[i]))
            return (NULL);
		i++;
	}
	if ((char)c == '\0' && s[i] == '\0')
		return (&s[i]);
	return (NULL);
}


char    valid_string_quot(char *str)
{
    char *tmp;
    char ret;

    if ((tmp = ft_strchr_blank((const char*)str, '\'')) != NULL && !ft_strchr_blank((const char*)str, '\"') )
    {
        if (*(tmp - 1) != '\\')
            return (*(tmp));
        else if (*(tmp - 1) == '\\')
        {
            if ((ret = valid_string_quot(tmp + 1)))
                return (ret);
        }
    }
    if ((tmp = ft_strchr_blank((const char*)str, '\"')) != NULL)
    {
        if (*(tmp - 1) != '\\')
            return (*(tmp));
        else if (*(tmp - 1) == '\\')
        {
            if ((ret = valid_string_quot(tmp + 1)))
                return (ret);
        }
    }
    return (0);
}