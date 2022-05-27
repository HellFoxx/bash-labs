#define _GNU_SOURCE 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>

#define NCHILD 2

int childPids[NCHILD];
int childIndex = 0;
volatile int mesNum;

char* now();
void parentSigHandle(int sig, siginfo_t *sigInfo, void *);
void childSigHandle(int sig);

int main() {

    struct sigaction sigact;
    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = parentSigHandle;
    sigaction(SIGUSR2, &sigact, NULL);

    // Игнорирование рассылки сигналов через kill
    signal(SIGUSR1, SIG_IGN);

    pid_t pid = 1;
    for (int i = 0; i < NCHILD && pid > 0; ++i) {
        switch (pid = fork()) {
            // error
            case -1: {
                perror("fork");
                for (int j = 0; j < i; ++j) {
                    kill(childPids[j], SIGKILL);
                }
                return -1;
            }
            // child
            case 0: {
                printf("CHILD%d - PID: %d - PPID: %d", childIndex, getpid(), getppid());
                putc('\n', stdout);
                signal(SIGUSR1, childSigHandle);
                break;
            }
            // parent
            default: {
                childPids[i] = pid;
                childIndex++;
                break;
            }
        }
    }

    if(pid > 0) {
        sleep(1);
        kill(0, SIGUSR1);
    }

    while(1);

}

char* now() {
    char *date;
    struct timeval tv;
    gettimeofday( &tv, NULL );
    asprintf(
        &date,
        "%02ld:%02ld:%02ld:%03ld",
        (tv.tv_sec / 3600 + 3) % 24,
        (tv.tv_sec / 60) % 60,
        tv.tv_sec % 60,
        tv.tv_usec / 1000
    );
    return date;
}

void logger(int childIndex, char *status, int sigNum) {
    printf(
        "%-5d %5d %5d %s - CHILD%d %3s SIGUSR%d\n",
        mesNum++,
        getpid(),
        getppid(),
        now(),
        childIndex,
        status,
        sigNum
    );
}

int getChildIndex(pid_t childPid) {
    for (int i = 0; i < NCHILD; ++i) {
        if(childPids[i] == childPid)
            return i;
    }
    return -1;
}

void parentSigHandle(int sig, siginfo_t *sigInfo, void *_) {
    logger(getChildIndex(sigInfo->si_pid), "put", 2);
    printf("PARENT get %s from CHILD%d\n", (sig == 12 ? "SIGUSR2" : ""), getChildIndex(sigInfo->si_pid));
    

    struct timespec t = {.tv_nsec = 100e6, .tv_sec = 0};
    nanosleep(&t, NULL);

    kill(0, SIGUSR1);
}

void childSigHandle(int sig) {
    logger(childIndex, "get", 1);
    kill(getppid(), SIGUSR2);
}
