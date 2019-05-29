#include <stdio.h>
#include <signal.h>

int sibling = 0;

void sigCathcher()
{
    printf("PID %d caught one \n", getpid());
    if (sibling != 0)
        kill(sibling, SIGINT);
    exit(0);
}


int main()
{
    int pid[5], i, deadChild;
    for (i = 0; i < 5; i++)
        pid[i] = 0;

    int Fpid = getpid();

    for (i = 0; i < 5; i++)
    {

        if ((pid[i] = fork()) < 0)
        {
            perror("Efork \n");
            exit(1);
        }
        else if (pid[i] == 0)
        {
            printf("PID %d ready \n", getpid());
            signal(SIGINT, sigCathcher);
            if (i >= 1)
                sibling = pid[i - 1];
            break;
        }
    }



    if (!pid[3] && getpid() != Fpid)
        pause();
    
    sleep(1);

    if (pid[3] && Fpid != getpid())
        kill(getpid(), SIGINT);

    sleep(1);
    if(Fpid == getpid())
    for (i = 0; i < 5; i++) {
        deadChild = wait(NULL);
        printf("Process %d is dead \n", deadChild);
    }

    return 0;
}