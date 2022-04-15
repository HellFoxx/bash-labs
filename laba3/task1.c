#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void printNow() {
    struct timeval tv;
    gettimeofday( &tv, NULL );
    printf("\t%02ld:%02ld:%02ld:%03ld\n",
        (tv.tv_sec / 3600 + 3) % 24,
        (tv.tv_sec / 60) % 60,
        tv.tv_sec % 60,
        tv.tv_usec / 1000
    );
}

void doFork() {
    pid_t pid = fork();
    int rv;
    switch(pid) {
        case -1: // error
            fprintf(stderr, "Fork error!");
            exit(1);
        case 0: // child
            printf("\n");
            printf("\tCHILD:\n");
            printf("\tPID - %d\n", getpid());
            printf("\tParent's PID - %d\n", getppid());
            printNow();
            //printf("Sleep 5 sec...");
            //sleep(5);
            printf("\n");
            exit(0);
        default: // parent
            //wait(0);
            printf("\n");
            printf("\tPARENT\n");
            printf("\tPID - %d\n", getpid());
            printf("\tParent's PID - %d\n", getppid());
            printNow();
            printf("\n");
            system("ps -x");
    }
}

int main() {
    
    doFork();
    doFork();
    while (1) {
        if (wait(0) == -1) {
            if (errno == ECHILD)
                break;
            fprintf(stderr, "Error while waiting child proc!\n");    
        }
    }

    return 0;

}