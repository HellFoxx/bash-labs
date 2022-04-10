#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <dirent.h>

void calcTaylorRow(char *filepath) {
    FILE *f = fopen(filepath, "r+t");
    char buffer[32];
    fgets(buffer, 30, f);
    int pid; float item; float sum = 0;
    while (fscanf(f, "%d\t%f\n", &pid, &item) != EOF)
        sum += item;
    fprintf(f, "Sum: %.15f", sum);
    fclose(f);
}

char* getFileName(char *filepath, int i) {
    char *num;
    char buffer[32];
    asprintf(&num, "%d", i);
    strcat(strcpy(buffer, filepath), num);
    strcat(buffer, ".txt");
    return buffer;
}

int fact(int x) {
    int res = 1;
    for (int i = 1; i <= x; i++)
        res *= i;
    return res;
}

float getTaylorItem(int n, int i, int k) {
    return pow(-1, k) * pow(2*3.14*i/n, 2*k+1) / fact(2*k+1);
}   

int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Error! Wrong number of params\n");
        return 1;
    }

    int n = strtol(argv[1], NULL, 10);
    if (n <= 0) {
        fprintf(stderr, "Error! Wrong 1st param\n");
        return 1;
    }
    int rowLen = strtol(argv[2], NULL, 10);
    if (rowLen <= 0) {
        fprintf(stderr, "Error! Wrong 2nd param\n");
        return 1;
    }

    pid_t pid;
    FILE *f;
    DIR* dir = opendir("results/");
    if (dir) {
        closedir(dir);
        system("rm -r results/");
    }
    system("mkdir results/");

    for (int i = 0; i < n; i++) {

        const char *filepath = "results/res";
        char *num;
        char buffer[32];
        asprintf(&num, "%d", i);
        strcat(strcpy(buffer, filepath), num);
        strcat(buffer, ".txt");
        f = fopen(buffer, "w");
        fprintf(f, "PID\t\tTaylor Item\n");
        fclose(f);

        for (int j = 0; j < rowLen; j++) {
            pid = fork();
            switch(pid) {
                case -1:
                    fprintf(stderr, "Fork error!");
                    exit(1);
                case 0:
                    f = fopen(buffer, "at");
                    fprintf(f, "%d\t%.15f\n", getpid(), getTaylorItem(n, i, j));
                    fclose(f);
                    exit(0);
                default:
                    waitpid(pid, 0);
            }
        }

        calcTaylorRow(buffer);
        
    }
    

}