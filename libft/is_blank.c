
#include "libft.h"

int is_blank(int c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (1);
    return (0);
}
