
#include "../../includes/sh.h"
#include <sys/wait.h>

int child_group;
int g_status;

void sig(int sig) {
    int status = 0;

    if (waitpid(child_group * -1, &status, WUNTRACED | WCONTINUED) != -1) {
        printf("[status] == [%d]\n", status);
        printf("SUSPEND(%d) (%d)\n", WIFSTOPPED(status), WSTOPSIG(status));
    }else 
        printf("-1\n");
    printf("child done!\n");
    g_status = status;
    // sleep(10000);
}

int main(int ac, char *av[])
{
    pid_t pid;
    int status;

    char *arv[3] = {"/bin/sleep", "68484", NULL};
    status = 0;
    if ((pid = fork()) < -1)
        return (2);
    if (pid == 0)
    {
        if (setpgrp() == -1)
            return (-1);
        if (execve(arv[0], arv, NULL) == -1)
            return (4);
        if (1)
        {
            printf("i enter\n");
            exit(0);
        }
    }
    else {
        signal(SIGCHLD, sig);
        child_group = pid;
        // if (tcsetpgrp(open(ttyname(0), O_RDWR), pid) == -1)
        //     printf("something wrong !!");   
        //sleep(10000);
        //  if (waitpid(pid * -1, &status, WUNTRACED | WCONTINUED) < 0)
        // {
        //     printf("error w_pid = %d\n", status);  
        //     return (1);
        // }
        if (wait(&status) < -1)
            return (3);
        if (WIFEXITED(status)) 
        {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }
        exit(EXIT_SUCCESS);
        //waitpid(pid, NULL, 0);
    }
    return (0);
}