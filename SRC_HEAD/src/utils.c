
#include "21sh.h"

int is_there_in_env(char *str, t_env **env_list)
{
    t_env *cur;

    cur = NULL;
    if (env_list)
    {
        cur = *env_list;
        while (cur)
        {
            if (ft_strequ(cur->env_var_name, str))
                return (1);
            cur = cur->next;
        }
    }
    return (0);
}

size_t wordinbuff_size(char *str)
{
    int i;
    size_t count;

    count = 0;
    i = 0;
    while(str[i] && (str[i] == '>' || str[i] == '<' || str[i] == '&')) // | &>- | &<- | > |  >>  | < | << | 2>&1 | >& | <& 
    {
        count++;
        i++;
    }
    while (str[i] && is_blank(str[i]))
        i++;
    while (str[i] && !is_blank(str[i]))
    {
        i++;
        count++;
    }
    return (count);
}

size_t wordinstr_size(char *str, size_t count)
{
    int i;
    size_t count_char = 0;

    i = 0;
    while(count)
    {
        while (str[i] && !is_blank(str[i]))
        {
            count_char++;
            i++;
        }
        while (str[i] && is_blank(str[i]))
            i++;
        count--;
    }
    return (count_char);
}

bool    is_quote(int c)
{
    if (c == '\'' || c == '\"')
        return (true);
    return (false);
}

// size_t last_node_check(t_lexer **tokenz, t_type type, int node_index)
// {
//     t_lexer *head;

//     if (tokenz && node_index)
//     {
//         if (!*tokenz)
//             return (0);
//         head = *tokenz;
//         while (head->next != NULL)
//         {
//             if (head->aggr_index)
//             {
//                 head->aggr_index = node_index;
//                 break ;
//             }
//             head = head->next;
//         }
//         if (head->type == type)
//             return (1);
//         return (0);
//     }
//     return (0);
// }

int check_quoting(t_lexer** head, t_type type, int cur_node_index)
{
    int i;
    t_lexer *lst;

    if (head && *head)
    {
        i = cur_node_index;
        lst = *head;
        while (lst && i)
        {
            if (ft_strequ(lst->data, ";"))
                break ;
            if (lst->type == type)
                return (1);
            lst = lst->next;
            i--;
        }
        return (0);
    }
    return (-1);
}

size_t ft_strlen_char(char *s, char c)
{
    size_t len;

    len = 0;
    while (s[len] != '\0' && s[len] != c)
        len++;
    return (len);
}

size_t ft_strlen_blank(char *s)
{
    size_t len;

    len = 0;
    while (s[len] != '\0' && s[len] != ' ' && s[len] != '\t')
        len++;
    return (len);
}

size_t ft_strlen_exp(char *s)
{
    size_t len;

    len = 0;
    while (s[len] != '\0' && !is_blank(s[len]))
    {
        if (len > 1 && is_quote(s[len]) && s[len - 1] != '\\')
            break ;
        else if (!len && is_quote(s[len]))
            break ;
        len++;
    }
    if (!len)
        len = 1;
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

int     valide_quote_check(char *str)
{
    char *pointer;
    char *second_pointer;

    pointer = ft_strchr((const char*)str, '\'');
    second_pointer = ft_strchr((const char*)str, '\"');

    if (pointer)
    {
        if (*(pointer - 1) != '\\')
            return (1);
    }
    else if (second_pointer)
    {
        if (*(second_pointer - 1) != '\\')
            return (1);
    }
    return (0);
}

char    valid_string_quot(char *str)
{
    char *tmp;
    char ret;

    ret = 0;
    if (!*str || !str)
        return (ret);
    if ((tmp = ft_strchr_blank((const char*)str, '\'')) != NULL)
    {
        if (*(tmp - 1) != '\\')
            return (*(tmp));
        else if (*(tmp - 1) == '\\')
        {
            if ((ret = valid_string_quot(str + 1)))
                return (ret);
        }
    }
    else if ((tmp = ft_strchr_blank((const char*)str, '\"')) != NULL)
    {
        if (*(tmp - 1) != '\\')
            return (*(tmp));
        else if (*(tmp - 1) == '\\')
        {
            if ((ret = valid_string_quot(str + 1)))
                return (ret);
        }
    }
    return (0);
}

size_t check_command_redir_size(char *buf)
{
    size_t i;

    i = 0;
    while(buf[i] && !is_blank(buf[i]) && buf[i] != '>' && buf[i] != '<' && (ft_isdigit(buf[i]) || buf[i] == '-'))
        i++;
    if (buf[i] == '>' || buf[i] == '<')
        return (1);
    else if (is_blank(buf[i]) || !ft_isdigit(buf[i]))
        return (0);
    return (0);
}


size_t ft_strchr_size(char *s, int c)
{
    size_t i = 0;
    while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if ((char)c == '\0' && *s == '\0')
		return (i);
	return (0);
}

char *sub_aggr_sym(char *str)
{
    int i;
    char *ret;

    ret = NULL;
    if (str && *str)
    {
        if (!(ret = ft_strnew(3)))
            return (NULL);
        if ((ft_is_there(str, '>') || ft_is_there(str, '<')))
        {
            i = 0;
            while (str[i] && (str[i] == '>' || str[i] == '<' || str[i] == '&' || str[i] == '-'))
            {
                ret[i] = str[i];
                i++;
            }
            return (ret);
        }
        return (ret);
    }
    return (ret);
}

char    *get_left_fd_(char *buf)
{
    int len;
    int i;
    char *ret;

    len = 0;
    while (ft_isdigit(buf[len]))
        len++;
    if (!len || !(ret = ft_strnew(len + 1)))
        return (NULL);
    i = 0;
    while (buf[i] && len)
    {
        ret[i] = buf[i];
        i++;
        len--;
    }
    return (ret);
}

int check_command_redir(t_lexer **head, char *buf, t_pointt *cor)
{
    int j;
    char tmp[MAX_INDEX];
    char mybuff[MAX_INDEX];
    char *ttmp;
    char *str_str;
    char *sym;

    if (!buf || !*buf)
        return (-1);
    str_str = ft_strdup(buf);   
    ttmp = str_str;
    size_t substr_size =  ft_strchr_size(ttmp, ' ');
    char *my_string = ft_strsub(buf, 0, substr_size);
    if (ft_is_there(buf, '>') || ft_is_there(buf, '<'))
    {
        j = 0;
        while (*(buf + j) && ft_isalnum(*(buf + j)) && buf[j] != '>' && buf[j] != '<')
        {
            tmp[j] = buf[j];
            j++;
        }
        tmp[j] = '\0';
        if (j)
            append_list_redi(head, ft_strdup(tmp), L_REDIR, cor);
        if ((buf[j] == '>' || buf[j] == '<' || buf[j] == '-') && (sym = sub_aggr_sym(buf + j)))
        {
            append_list_redi(head, ft_strdup(sym), AGGR_SYM, cor);
            int sym_len = (int)ft_strlen(sym);
            ft_strdel(&sym);
            ft_strdel(&my_string);
            ft_strdel(&str_str);
            return (j + sym_len + 1);
        }
        else if (ft_is_there(";", buf[j]))
            return (1);
        ft_strdel(&my_string);
        ft_strdel(&str_str);
        return (j);
    }
    return (1);
}

static size_t calc_size(char * str)
{
    size_t count;
    int i;

    count = 0;
    if (str && *str)
    {
        i = 0;
        while (str[i] && is_blank(str[i]))
            i++;
        while (str[i] && !is_blank(str[i]))
        {
            count++;
            i++;
        }
        return (count);
    }
    return (-1);
}

size_t calc_size_right_redir(char * str)
{
    size_t count;
    int i;

    count = 0;
    if (str && *str)
    {
        i = 0;
        while (str[i] && is_blank(str[i]))
            i++;
        while (str[i] && !is_blank(str[i]))
        {
            count++;
            i++;
        }
        return (i);
    }
    return (-1);
}

char    *get_right_redir(char *str)
{
    int i;
    int j;
    char *ret;


    ret = NULL;
    if (str && *str)
    {
        i = 0;
        if (!(ret = ft_strnew(calc_size(str + i))))
            return (NULL);
        while (str[i] && is_blank(str[i]))
            i++;
        j = 0;
        while (str[i] && !is_blank(str[i]))
        {
            ret[j] = str[i];
            j++;
            i++;
        }
        return (ret);
    }
    return (ret);
}

int ft_is_aggr(char c)
{
    if (c == '>' || c == '<')
        return (1);
    return (0);
}

int check_if_is_aggr(t_lexer **root) // this function is the problem!!
{
    t_lexer *cur;

    if (root)
    {
        cur = *root;
        while (cur->next)
            cur = cur->next;
        if (cur->type == AGGR_SYM)
            return (1);        
    }
    return (0);
}

t_lexer *get_last_node(t_lexer **head)
{
    t_lexer *current;

    current = NULL;
    if (head && *head)
    {
        current = *head;
        while (current->next)
            current = current->next;
    }
    return (current);
}