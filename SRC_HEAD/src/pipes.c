#include "21sh.h"

int main() {
    char *argv1[3] = {
        "/bin/sleep",
        "1000",
        NULL
    };
    char *argv2[3] = {
        "/bin/sleep",
        "1000",
        NULL
    };
    int pip[2];

    pipe(pip);
    if (!fork()) {
        close(pip[0]);
        dup2(pip[1], 1);
        close(pip[1]);
        execve(argv1[0], argv1, NULL);
        exit(1);
    }
    int tmpp = pip[0];
    close(pip[1]);
    pipe(pip);
    if (!fork()) {
        close(pip[1]);
        dup2(tmpp, 0);
        close(tmpp);
        close(pip[0]);
        execve(argv2[0], argv2, NULL);
        exit(1);
    }
    close(pip[1]);
    close(pip[0]);
    close(tmpp);
    while (wait(NULL) > 0){}
    return (0);
}