


#include "21sh.h"

int main(int ac, char *av[])
{
    int id = fork();

    if (id == 0)
        printf("im the childe process and my pid id %d\n", getpid());
    else
        printf("im the parent process and my pid id %d\n", getpid());
    if (id != 0)
        wait(NULL);
    return 0;
}