


#include "21sh.h"

int main(int ac, char *av[])
{
    int id = fork();
    int id2 = fork();

    if (id == 0)
    {
        if (id2 == 0)
            printf("[Y] im the childe of the child process and my pid is %d\n", getpid());
        else
            printf("[X] im the childe of the parent process and my id is %d\n", getpid());
    }
    else 
    {
        if (id2 == 0)
            printf("[Z] im the childe of the parent process and my id is %d\n", getpid());
        else
            printf("[P] im the parent process! %d\n", getpid());
    }
    while (wait(NULL) != -1);
    return 0;
}