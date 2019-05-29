//#include "md5.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 20
char secret[33];
int pid;
void sigqut()
{
    exit(0);
}

void sigint()
{
    char check = secret[0];
    int i = 0;
    while (check)
    {
        check = secret[++i];
    }
    if (i == 32)
    {
        printf("encrypted by process %d : %s \n", getpid(), secret);
        kill(pid, SIGQUIT);
    }
}

int main()
{
    signal(SIGINT, sigint);
    char msg[SIZE];
    char buffer[SIZE];
    int pipeParent[2], pipeChild[2], i;

    int checkPipeP = pipe(pipeParent);
    int checkPipeC = pipe(pipeChild);

    if (checkPipeP < 0 || checkPipeC < 0)
    {
        printf("Unable open pipe \n");
        exit(1);
    }

    if ((pid = fork()) < 0)
    {
        perror("Unable fork \n");
        exit(1);
    }

    if (pid)
    {
        printf("write something: ");
        fgets(msg, SIZE, stdin);
        write(pipeParent[1], msg, SIZE);
        read(pipeChild[0], secret, 33);

        pause();
    }
    else
    {
        signal(SIGQUIT, sigqut);
        read(pipeParent[0], buffer, SIZE);
        char rs[33] = "Back to parent34af a fadfettfgas";
        write(pipeChild[1], rs, 33);
        sleep(2);
        kill(getppid(), SIGINT);
        pause();
    }

    return 0;
}